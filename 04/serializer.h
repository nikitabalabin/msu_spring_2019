#pragma once
#include <iostream>

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
	static constexpr char Separator = ' ';

public:

	explicit Serializer(std::ostream& out): 
		out_(out) {}

	template <class T>
	Error save(T& object)
	{
		return object.serialize(*this);
	}

	template <class... ArgsT>
    Error operator()(ArgsT&&... args)
    {
        return process(std::forward<ArgsT>(args)...);
    }

private:

	std::ostream& out_;

    template <class T, class... ArgsT>
    Error process(T && value, ArgsT&& ... args) 
    {
        if (process(std::forward<T>(value)) == Error::NoError)
            return process(std::forward<ArgsT>(args)...);

        return Error::CorruptedArchive;
    } 

    Error process(bool value)
    {
        if (value)
        	if (out_ << "true" << Separator)
            	return Error::NoError;
        else
        	if (out_ << "false" << Separator)
            	return Error::NoError; 

        return Error::CorruptedArchive;
    }

    Error process(uint64_t n)
    {
        if (out_ << n << Separator)
            return Error::NoError;

        return Error::CorruptedArchive;
    }

    Error process() 
    {
        return Error::CorruptedArchive;
    }

};


class Deserializer
{
  
public:

    explicit Deserializer(std::istream& in):
    in_(in) {}

    template <class T>
    Error load(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args)
    {
        return process(std::forward<ArgsT>(args)...);
    }

private:

	std::istream& in_;

    template <class T, class... ArgsT>

    Error process(T && value, ArgsT && ... args)
    {
        if (process(std::forward<T> (value)) == Error::NoError)
            return process(std::forward<ArgsT>(args)...);

        return Error::CorruptedArchive;
    }    

    Error process(bool& value)
    {
        std::string text;

        if (in_ >> text)
        {
            if (text == "true")
                value = true;
            else if (text == "false")
                value = false;
            else
                return Error::CorruptedArchive;

            return Error::NoError;
        }

        return Error::CorruptedArchive;
    }
    
    Error process(uint64_t& value)
    {
        std::string text;

        if (in_ >> text)
        {
            if (text[0] == '-')
	        	return Error::CorruptedArchive;

            try
            {
                value  = stoul(text);
            }
            catch (const std::logic_error & error)
            {
                return Error::CorruptedArchive;
            }

            return Error::NoError;
        }

        return Error::CorruptedArchive;
    }

    Error process() 
    {
        return Error::CorruptedArchive;
    }
};