#pragma once
#include <iostream>
#include <string>
#include <cstdint>

enum class Error {
    NoError,
    CorruptedArchive
};

class Serializer {
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream& out)
        : out_(out) {}

    template <class T>
    Error save(T& object) {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args) {
        return process(std::forward<ArgsT>(args)...);
    }

private:
    std::ostream& out_;

    template<class T>
    Error process(T&& object) {
        print(std::forward<T>(object));
        return Error::NoError;
    }

    template<class T, class... ArgsT>
    Error process(T&& object, ArgsT&&... args) {
        print(std::forward<T>(object));
        out_ << Separator;
        return process(std::forward<ArgsT>(args)...);
    }

    template<class T>
    void print(T&& object) {
        out_ << object;
    }

    void print(bool object) {
        if (object)
            out_ << "true";
        else
            out_ << "false";
    }
};

class Deserializer {
public:
    explicit Deserializer(std::istream& in)
        : in_(in) {}

    template <class T>
    Error load(T& object) {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args) {
        return process(std::forward<ArgsT>(args)...);
    }

private:
    std::istream& in_;
    
    template <class T>
    Error process(T&& object){
        if (get(std::forward<T>(object)) == Error::CorruptedArchive)
            return Error::CorruptedArchive;
        else
            return Error::NoError;
    }

    template <class T, class... ArgsT>
    Error process(T&& object, ArgsT&&... args){
        if (get(std::forward<T>(object)) == Error::CorruptedArchive)
            return Error::CorruptedArchive;
        else
            return process(std::forward<ArgsT>(args)...);
    }

    Error get(uint64_t& value) {
        std::string text;
        in_ >> text;
        if (text[0] == '-')
            return Error::CorruptedArchive;
        else try {
            value = std::stoul(text);
            return Error::NoError;
        } catch (std::invalid_argument& e) { 
            return Error::CorruptedArchive; 
        }
    }

    Error get(bool& value) {
        std::string text;
        in_ >> text;
        if (text == "true")
            value = true;
        else if (text == "false")
            value = false;
        else
            return Error::CorruptedArchive;
        return Error::NoError;
    }
};
