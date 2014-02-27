#ifndef IMAGE_PREVIEW_HASH_H
#define IMAGE_PREVIEW_HASH_H

#include <functional>
#include <hawk/handlers/hash.h>
#include <hawk/calchash.h>
#include "previews/Image_preview.h"

static std::string type { "image" };
static size_t hash = hawk::calculate_mime_hash(type);

namespace hawk {
	template <>
	size_t get_handler_hash<Image_preview>()
	{
		return hash;
	}
}

#endif // IMAGE_PREVIEW_HASH_H
