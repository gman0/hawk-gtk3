#ifndef TEXT_PREVIEW_HASH_H
#define TEXT_PREVIEW_HASH_H

#include <functional>
#include <hawk/handlers/hash.h>
#include <hawk/calchash.h>
#include "previews/Text_preview.h"

static std::string type { "text" };
static size_t hash = hawk::calculate_mime_hash(type);

namespace hawk {
	template <>
	size_t get_handler_hash<Text_preview>()
	{
		return hash;
	}
}

#endif // TEXT_PREVIEW_HASH_H
