#pragma once

#ifndef SPDLOG_HEADER_ONLY
#    include "DUOLCommon/Log//DUOLSink.h"
#endif
#include <spdlog/common.h>

#include <spdlog/details/os.h>
#include <spdlog/details/circular_q.h>
#include <spdlog/fmt/chrono.h>
#include <spdlog/details/file_helper.h>
#include <spdlog/sinks/base_sink.h>

#include <spdlog/fmt/fmt.h>
#include <spdlog/details/synchronous_factory.h>

#include <cerrno>
#include <ctime>
#include <tuple>

namespace duollog
{
	namespace sinks {

		template <typename Mutex>
		SPDLOG_INLINE duollog::sinks::DUOLSink<Mutex>::DUOLSink(spdlog::filename_t base_filename, std::size_t max_size, std::size_t max_files,
			const spdlog::file_event_handlers& event_handlers)
			: _baseFilename(std::move(base_filename))
			, _maxSize(max_size)
			, _maxFiles(max_files)
			, _fileHelper{ event_handlers }
		{
			if (max_size == 0)
			{
				spdlog::throw_spdlog_ex("rotating sink constructor: max_size arg cannot be zero");
			}

			if (max_files > 200000)
			{
				spdlog::throw_spdlog_ex("rotating sink constructor: max_files arg cannot exceed 200000");
			}
			// 일단 이름을 먼저 날짜로 만들어준다.
			// 시간을 가져온다. 
			auto now = spdlog::log_clock::now();
			// 이름뒤에 합친다.

			auto filename = DailyFileName(_baseFilename, NowTime(now));
			_baseFilename = filename;
			_fileHelper.open(filename, false);
			_rotationTp = NextRotationTp();

			// 크기도 체크해준다. 
			_currentSize = _fileHelper.size();

			// 일단 임의로 시간을 설정해준다. 
			rotationHour = 2;
			rotationMin = 30;

			if (_currentSize > 0)
			{
				RotateFile();
				_currentSize = 0;
			}
		}

		template <typename Mutex>
		SPDLOG_INLINE void duollog::sinks::DUOLSink<Mutex>::sink_it_(const spdlog::details::log_msg& msg)
		{
			// 날짜로 이름을 만든다. 
			//auto time = msg.time;
			//bool shouldrotate = time >= _rotationTp;
			//if (shouldrotate)
			//{
			//	// 이름을 다시 설정하고 파일을 만든다. 
			//	auto filename = DailyFileName(_baseFilename, NowTime(time));
			//	_fileHelper.open(filename, false);
			//	_rotationTp = NextRotationTp();
			//}
			spdlog::memory_buf_t formatted;
			spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);

			// 사이즈를 비교해서 사이즈가 크면 새 파일을 만든다. 
			auto newsize = _currentSize + formatted.size();

			if (newsize > _maxSize)
			{
				_fileHelper.flush();
				if (_fileHelper.size() > 0)
				{
					RotateFile();
					newsize = formatted.size();
				}
			}
			_fileHelper.write(formatted);
			_currentSize = newsize;

		}

		template <typename Mutex>
		SPDLOG_INLINE spdlog::filename_t duollog::sinks::DUOLSink<Mutex>::CountFileName(const spdlog::filename_t& filename, std::size_t index)
		{
			if (index == 0u)
			{
				return filename;
			}

			spdlog::filename_t basename, ext;
			std::tie(basename, ext) = spdlog::details::file_helper::split_by_extension(filename);
			return spdlog::fmt_lib::format(SPDLOG_FILENAME_T("{}_{}{}"), basename, index, ext);
		}

		template <typename Mutex>
		SPDLOG_INLINE spdlog::filename_t duollog::sinks::DUOLSink<Mutex>::DailyFileName(const spdlog::filename_t& filename, const tm& now_tm)
		{
			spdlog::filename_t basename, ext;

			// 날짜별로 파일을 생성하기 위한 코드
			std::size_t filepath = filename.find('/');
			std::string createfilename = filename.substr(0,filepath);
			std::string originfilename = filename.substr(filepath, filename.size());
			std::string yeardate = FormatDate(now_tm);
			std::string dailyfilename = createfilename+"/"+yeardate;
			const spdlog::filename_t sumfilename = dailyfilename + originfilename;

			std::tie(basename, ext) = spdlog::details::file_helper::split_by_extension(sumfilename);
			return spdlog::fmt_lib::format(SPDLOG_FMT_STRING(SPDLOG_FILENAME_T("{}_{:04d}-{:02d}-{:02d}{}")), basename, now_tm.tm_year + 1900,
				now_tm.tm_mon + 1, now_tm.tm_mday, ext);
		}

		template <typename Mutex>
		SPDLOG_INLINE spdlog::filename_t duollog::sinks::DUOLSink<Mutex>::filename()
		{
			std::lock_guard<Mutex> lock(spdlog::sinks::base_sink<Mutex>::mutex_);
			return _fileHelper.filename();
		}

		template <typename Mutex>
		SPDLOG_INLINE void duollog::sinks::DUOLSink<Mutex>::flush_()
		{
			_fileHelper.flush();
		}

		template <typename Mutex>
		SPDLOG_INLINE void duollog::sinks::DUOLSink<Mutex>::RotateFile()
		{
			using spdlog::details::os::filename_to_str;
			using spdlog::details::os::path_exists;

			_fileHelper.close();
			for (auto i = _maxFiles; i > 0; --i)
			{
				// 이름을 DUOL_2023-02-03_1이런식으로 만든다.
				spdlog::filename_t src = CountFileName(_baseFilename, i - 1);
				if (!path_exists(src))
				{
					wprintf(L"Format message failed with 0x%x\n", GetLastError());
					continue;
				}
				spdlog::filename_t target = CountFileName(_baseFilename, i);

				if (!RenameFile(src, target))
				{
					// if failed try again after a small delay.
					// this is a workaround to a windows issue, where very high rotation
					// rates can cause the rename to fail with permission denied (because of antivirus?).
					spdlog::details::os::sleep_for_millis(100);
					if (!RenameFile(src, target))
					{
						_fileHelper.reopen(true); // truncate the log file anyway to prevent it to grow beyond its limit!
						_currentSize = 0;
						spdlog::throw_spdlog_ex("rotating_file_sink: failed renaming " + filename_to_str(src) + " to " + filename_to_str(target), errno);
					}
				}
			}
			_fileHelper.reopen(true);
		}

		template <typename Mutex>
		SPDLOG_INLINE bool duollog::sinks::DUOLSink<Mutex>::RenameFile(const spdlog::filename_t& src_filename,
			const spdlog::filename_t& target_filename)
		{
			// try to delete the target file in case it already exists.
			(void)spdlog::details::os::remove(target_filename);
			return spdlog::details::os::rename(src_filename, target_filename) == 0;
		}

		template <typename Mutex>
		SPDLOG_INLINE tm duollog::sinks::DUOLSink<Mutex>::NowTime(spdlog::log_clock::time_point tp)
		{
			time_t tnow = spdlog::log_clock::to_time_t(tp);
			return spdlog::details::os::localtime(tnow);
		}

		template <typename Mutex>
		SPDLOG_INLINE spdlog::log_clock::time_point duollog::sinks::DUOLSink<Mutex>::NextRotationTp()
		{
			auto now = spdlog::log_clock::now();
			tm date = NowTime(now);
			date.tm_hour = rotationHour;
			date.tm_min = rotationMin;
			date.tm_sec = 0;
			auto rotation_time = spdlog::log_clock::from_time_t(std::mktime(&date));
			if (rotation_time > now)
			{
				return rotation_time;
			}
			return { rotation_time + std::chrono::hours(24) };
		}

		template <typename Mutex>
		SPDLOG_INLINE std::string DUOLSink<Mutex>::FormatDate(const tm& now_tm)
		{
			char buffer[11];
			std::strftime(buffer, sizeof buffer, "%Y-%m-%d", &now_tm);
			return buffer;
		}
	}
}