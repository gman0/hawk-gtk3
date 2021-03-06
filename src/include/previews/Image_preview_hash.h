/*
	Copyright (C) 2013-2014 Róbert "gman" Vašek <gman@codefreax.org>

	This file is part of hawk-gtk3.

	hawk-gtk3 is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 2 of the License, or
	(at your option) any later version.

	hawk-gtk3 is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with hawk-gtk3.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef IMAGE_PREVIEW_HASH_H
#define IMAGE_PREVIEW_HASH_H

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
