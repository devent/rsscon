/*
 * Copyright 2012 Erwin Müller <erwin.mueller@deventm.org>
 *
 * This file is part of rsscon-jni.
 *
 * rsscon-jni is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * rsscon-jni is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with rsscon-jni. If not, see <http://www.gnu.org/licenses/>.
 */
package com.anrisoftware.rsscon.nativeimpl;

import com.anrisoftware.nativeloader.api.AOL;
import com.anrisoftware.nativeloader.api.LibraryNameMapper;

/**
 * Maps the library {@code log4c} to the Linux name {@code liblog4c.so.3}.
 * 
 * @author Erwin Mueller, erwin.mueller@deventm.org
 * @since 1.1
 */
class Log4cLibraryNameMapper implements LibraryNameMapper {

	static final String LIBRARY_NAME = "log4c";

	@Override
	public String mapLibraryName(AOL aol, String name) {
		if (name.equals(LIBRARY_NAME)) {
			if (aol.getOs().equals("Linux")) {
				return "liblog4c.so.3";
			}
		}
		return null;
	}

}
