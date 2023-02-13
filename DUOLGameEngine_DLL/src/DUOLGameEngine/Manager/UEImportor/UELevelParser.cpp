#include "DUOLGameEngine/Manager/UEImportor/UELevelParser.h"

namespace DUOLGameEngine
{
    void ParseObject(std::stringstream& _stream, int _startPos, int _endPos, UELevelParser::Actor& _ownerRef)
    {
        std::string _name;
        std::string _className;
        auto _iter = _ownerRef.components.end();

        while (true)
        {
            std::string _token;
            _stream >> _token;

            const int _pos = (int)_stream.tellg();
            if (_pos >= _endPos)
                break;

            if (_token.find("Class") == 0)
            {
                _className = UELevelParser::Property_PathExtension::Parse(_token).value;
            }
            else if (_token.find("Name") == 0)
            {
                assert(_name.empty());

                // Name 토큰 내용을 읽습니다.
                // Name 토큰의 값을 감싸는 쌍따옴표 ("")를 제거한 형태로 저장합니다.
                std::string _name = _token.substr(std::string{ "Name" }.size() + 1);
                _name = UELevelParser::Property_QuotedString::Parse(_name).value;

                // 이미 있는 오브젝트인지 확인합니다.
                _iter = std::find_if(_ownerRef.components.begin(), _ownerRef.components.end(),
                    [&_name](UELevelParser::Object c) { return c.name.find(_name) == 0; });
                if (_iter == _ownerRef.components.end())
                {
                    // 없다면 추가합니다.
                    UELevelParser::Object _object{ _name, _className };
                    _ownerRef.components.push_back({ _object });
                    _iter = _ownerRef.components.end() - 1;
                }
            }
            else if (_token.find("Archetype") == 0
                || _token.find("ActorFolderPath") == 0)
            {
                // Archetype, ActorFolderPath 토큰은 무시합니다.
            }
            else if (_token.find("CustomProperties") == 0)
            {
                // CustomProperties 토큰은 대응하는 값을 가지고 있지 않으므로 무시합니다.
                continue;
            }
            else
            {
                // 그 외 토큰들의 값을 읽어 저장합니다.
                size_t _dividerPos = _token.find('=');

                if (_dividerPos > _token.size())
                {
                    // 값이 없는 토큰인 경우 무시합니다.
                    continue;
                }

                auto _key = _token.substr(0, _dividerPos);
                auto _value = _token.substr(_dividerPos + 1);
                (*_iter).properties.insert({ _key, _value });
            }
        }
    }

    void ParseActor(std::stringstream& _stream, int _startPos, int _endPos, UELevelParser::Level& _levelRef)
    {
        UELevelParser::Actor _actor;

        while (true)
        {
            std::string _token;
            _stream >> _token;

            const int _pos = (int)_stream.tellg();
            if (_pos >= _endPos)
                break;

            if (_token == "Begin")
            {
                std::string _beginForWhat;
                _stream >> _beginForWhat;

                if (_beginForWhat == "Object") // Begin Object
                {
                    std::size_t _start = static_cast<int>(_stream.tellg()) + 1;
                    std::size_t _end = _stream.str().find("End Object", _start);
                    assert(_end < _endPos);

                    ParseObject(_stream, _start, _end, _actor);
                    _stream.seekg(_end);
                } // End Object
            }
            else if (_token.find("Name") == 0)
            {
                // Name 토큰 내용을 읽습니다.
                std::string _name = _token.substr(std::string{ "Name" }.size() + 1);
                _actor.name = _name;
            }
            // 부모 개체
            else if (_token.find("ParentActor") == 0)
            {
                std::string _parentName = _token.substr(std::string{ "ParentActor" }.size() + 1);

                _actor.parentActorName = _parentName;
            }
            else
            {
                // 그 외 Class, Archetype, ActorFolderPath 등의 토큰을 무시합니다.
            }
        }

        _levelRef.actors.push_back(_actor);
    }

    UELevelParser::Level ParseLevel(std::stringstream& _stream, int _startPos, int _endPos)
    {
        UELevelParser::Level _level;

        while (true)
        {
            std::string _token;
            _stream >> _token;

            const int _pos = (int)_stream.tellg();
            if (_pos >= _endPos)
                break;

            if (_token == "Begin")
            {
                std::string _beginForWhat;
                _stream >> _beginForWhat;

                if (_beginForWhat == "Actor") // Begin Actor
                {
                    std::size_t _start = static_cast<int>(_stream.tellg()) + 1;
                    std::size_t _end = _stream.str().find("End Actor", _start);
                    assert(_end < _endPos);

                    ParseActor(_stream, _start, _end, _level);
                    _stream.seekg(_end);
                } // End Actor
            }
        }

        return _level;
    }

    UELevelParser::Level UELevelParser::Parse(std::stringstream& _stream)
    {
        UELevelParser::Level _level;
        int _endPos = 99999999;

        std::string _token;
        _stream >> _token; // Begin
        _stream >> _token; // Map

        _stream >> _token;
        if (_token == "Begin")
        {
            std::string _beginForWhat;
            _stream >> _beginForWhat;

            if (_beginForWhat == "Level") // Begin Level
            {
                std::size_t _start = static_cast<int>(_stream.tellg()) + 1;
                std::size_t _end = _stream.str().find("End Level", _start);
                assert(_end < _endPos);

                _level = ParseLevel(_stream, _start, _end);
            } // End Level
        }

        return _level;
    }

    std::ostream& operator<<(std::ostream& os, const DUOLMath::Vector3& obj)
    {
        return os
            << "x: " << obj.x
            << " y: " << obj.y
            << " z: " << obj.z;
    }

    std::ostream& operator<<(std::ostream& os, const UELevelParser::Property_String& obj)
    {
        return os << "value: " << obj.value;
    }

    std::ostream& operator<<(std::ostream& os, const UELevelParser::Property_QuotedString& obj)
    {
        return os << "value: " << obj.value;
    }

    std::ostream& operator<<(std::ostream& os, const UELevelParser::Property_Bool& obj)
    {
        return os << "value: " << obj.value;
    }

    std::ostream& operator<<(std::ostream& os, const UELevelParser::Property_Vector& obj)
    {
        return os << "value: " << obj.value;
    }

    std::ostream& operator<<(std::ostream& os, const UELevelParser::Property_Map& obj)
    {
        return os
            << "key: " << obj.key
            << ", value: " << obj.value;
    }

    std::ostream& operator<<(std::ostream& os, const UELevelParser::Component_StaticMeshComponent& obj)
    {
        return os
            << "meshFileNameWithoutExtension: " << obj.meshFileNameWithoutExtension
            << ", relativeLocation: " << obj.relativeLocation
            << ", relativeScale3D: " << obj.relativeScale3D;
    }

    indent_ostream& operator<<(indent_ostream& os, const UELevelParser::Object& obj)
    {
        os << "name: " << obj.name << ",\n";
        os.print_indent();
        os << "properties: " << obj.properties;
        return os;
    }

    indent_ostream& operator<<(indent_ostream& os, const UELevelParser::Actor& obj)
    {
        os << "name: " << obj.name << ",\n";
        os.print_indent();
        os << "components: " << obj.components;
        return os;
    }

    indent_ostream& operator<<(indent_ostream& os, const UELevelParser::Level& obj)
    {
        return os << "actors: " << obj.actors;
    }
}