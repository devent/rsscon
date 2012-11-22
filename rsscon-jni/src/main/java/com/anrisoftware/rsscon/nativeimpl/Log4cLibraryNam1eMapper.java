package com.anrisoftware.rsscon.nativeimpl;

import com.anrisoftware.nativeloader.api.AOL;
import com.anrisoftware.nativeloader.api.LibraryNameMapper;

/**
 * Maps the library {@code log4c} to the Linux name {@code liblog4c.so.3}.
 * 
 * @author Erwin Mueller, erwin.mueller@deventm.org
 * @since 1.1
 */
class Log4cLibraryNam1eMapper implements LibraryNameMapper {

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
