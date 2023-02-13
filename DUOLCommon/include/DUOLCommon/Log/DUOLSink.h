#pragma once
#include <spdlog/details/file_helper.h>
#include <spdlog/details/circular_q.h>

#include "spdlog/sinks/base_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/spdlog.h"
#include "spdlog/details/null_mutex.h"

#include <chrono>
#include <mutex>
#include <string>

namespace duollog
{
	namespace sinks
	{
		template<typename Mutex>
		class DUOLSink final :public spdlog::sinks::base_sink<Mutex>
		{
		public:
			DUOLSink(spdlog::filename_t base_filename, std::size_t max_size, std::size_t max_files, const spdlog::file_event_handlers& event_handlers = {});

			static spdlog::filename_t CountFileName(const spdlog::filename_t& filename, std::size_t index);

			static spdlog::filename_t DailyFileName(const spdlog::filename_t& filename, const tm& now_tm);

			spdlog::filename_t filename();

		protected:
			void sink_it_(const spdlog::details::log_msg& msg) override;

			void flush_() override;

		private:
			// Rotate files:
		   // log.txt -> log.1.txt
		   // log.1.txt -> log.2.txt
		   // log.2.txt -> log.3.txt
		   // log.3.txt -> delete
			void RotateFile();

			// delete the target if exists, and rename the src file  to target
		  // return true on success, false otherwise.
			bool RenameFile(const spdlog::filename_t& src_filename, const spdlog::filename_t& target_filename);

			tm NowTime(spdlog::log_clock::time_point tp);

			spdlog::log_clock::time_point NextRotationTp();

			static std::string FormatDate(const tm& now_tm);
		/*	inline std::vector<spdlog::sink_ptr>& spdlog::logger::sinks()
			{
				return sinks_;
			}*/

			spdlog::filename_t _baseFilename;
			std::size_t _maxSize;
			std::size_t _maxFiles;
			std::size_t _currentSize;
			std::size_t _currentExpand;
			std::chrono::system_clock::time_point _rotationTp;
			int rotationHour;
			int rotationMin;
			uint16_t max_files_;
			spdlog::details::circular_q<spdlog::filename_t> _initFileName;
			spdlog::details::file_helper _fileHelper;
		};

		using duol_file_sink_mt = DUOLSink<std::mutex>;
		using duol_file_sink_st = DUOLSink<spdlog::details::null_mutex>;

	}


	//
	// factory functions
	//

	template<typename Factory = spdlog::synchronous_factory>
	inline std::shared_ptr<spdlog::logger> duol_logger_mt(const std::string& logger_name, const spdlog::filename_t& filename, size_t max_file_size,
	                                                          size_t max_files, const spdlog::file_event_handlers& event_handlers = {})
	{
		return Factory::template create<sinks::duol_file_sink_mt>(
			logger_name, filename, max_file_size, max_files,  event_handlers);
	}

	template<typename Factory = spdlog::synchronous_factory>
	inline std::shared_ptr<spdlog::logger> duol_logger_st(const std::string& logger_name, const spdlog::filename_t& filename, size_t max_file_size,
	                                                          size_t max_files,  const spdlog::file_event_handlers& event_handlers = {})
	{
		return Factory::template create<sinks::duol_file_sink_st>(
			logger_name, filename, max_file_size, max_files, event_handlers);
	}
} // namespace spdlog

#ifdef SPDLOG_HEADER_ONLY
#include "DUOLCommon/Log/DUOLSink-ini.h"
#endif

