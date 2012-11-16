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
package com.anrisoftware.rsscon.utils

import static com.anrisoftware.globalpom.utils.TestUtils.*

import org.junit.BeforeClass
import org.slf4j.LoggerFactory

import com.anrisoftware.globalpom.utils.TestUtils
import com.anrisoftware.propertiesutils.ContextProperties
import com.anrisoftware.rsscon.api.NativeRssconInputStreamFactory
import com.anrisoftware.rsscon.api.NativeRssconOutputStreamFactory
import com.anrisoftware.rsscon.api.RssconNativeFactory
import com.anrisoftware.rsscon.nativeimpl.NativeRssconModule
import com.google.inject.Guice
import com.google.inject.Injector

/**
 * Creates the factories for the native rsscon.
 *
 * @author Erwin Mueller, erwin.mueller@deventm.org
 * @since 1.0
 */
class RssconTestUtils {

	/*
	 * -Dcom.anrisoftware.rsscon.utils.rsscon_available = true
	 * -Dcom.anrisoftware.rsscon.utils.rsscon_path = /dev/ttyUSB0
	 * LD_LIBRARY_PATH
	 * ${rsscon_jni_ldpath}
	 */

	/**
	 * The device path.
	 */
	static String devicePath

	/**
	 * If the device is available.
	 */
	static boolean deviceAvailable

	/**
	 * Some device paths.
	 */
	static devicePaths = ["/dev/ttyUSB0"]

	static Injector injector

	static NativeRssconInputStreamFactory inputFactory

	static NativeRssconOutputStreamFactory outputFactory

	static RssconNativeFactory nativeFactory

	@BeforeClass
	static void setupToStringStyle() {
		TestUtils.toStringStyle
	}

	@BeforeClass
	static void checkDeviceAvailable() {
		def log = LoggerFactory.getLogger(RssconTestUtils)
		def p = new ContextProperties(RssconTestUtils, System.properties)
		deviceAvailable = p.getBooleanProperty("rsscon_available", false)
		devicePath = p.getProperty("rsscon_path", devicePaths[0])
		if (!deviceAvailable) {
			log.warn "Rsscon device is not available for testing."
		} else {
			log.debug "Rsscon device available for testing with path {}.", devicePaths
		}
	}

	@BeforeClass
	static void checkLibraryPathSet() {
		def log = LoggerFactory.getLogger(RssconTestUtils)
		def libraryPath = System.getProperty("java.library.path")
		def pathSep = System.getProperty("path.separator")
		def customLibraryPath = libraryPath.split(pathSep).find { it.endsWith("customlibs") }
		if (customLibraryPath == null) {
			log.warn 'Custom library is expected for test: Set LD_LIBRARY_PATH=${workspace_loc}/customlibs.'
		}
	}

	@BeforeClass
	static void createFactories() {
		injector = createInjector()
		inputFactory = injector.getInstance NativeRssconInputStreamFactory
		outputFactory = injector.getInstance NativeRssconOutputStreamFactory
		nativeFactory = injector.getInstance RssconNativeFactory
	}

	static Injector createInjector() {
		Guice.createInjector(new NativeRssconModule())
	}
}
