#pragma once
#include <json_parser.hpp>
#include <ptree.hpp>
#include "VKPhoto.hpp"

using JSON = boost::property_tree::ptree;

class VKObject
{
private:
	JSON object = {};
	std::string type = "error";
public:
	VKObject(JSON toParse)
	{
		object = toParse;

		try
		{
			type = object["type"];
		}
		catch (...)
		{
			type = "error";
		}
	}

	bool IsPhoto()
	{
		return type == "photo";
	}

	VKPhoto MakePhoto()
	{
		return VKPhoto(object);
	}
};