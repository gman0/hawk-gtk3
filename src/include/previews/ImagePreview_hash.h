#ifndef IMAGE_PREVIEW_HASH_H
#define IMAGE_PREVIEW_HASH_H

#include <functional>
#include <hawk/handlers/hash.h>
#include "previews/ImagePreview.h"

static std::string type { "image/jpeg" };
static size_t hash = std::hash<std::string>()(type);

namespace hawk {
	template <>
	size_t get_handler_hash<Image_preview>()
	{
		return hash;
	}
}

#endif // IMAGE_PREVIEW_HASH_H
