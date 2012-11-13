package com.anrisoftware.rsscon.nativeimpl

import org.junit.After
import org.junit.Before
import org.junit.BeforeClass
import org.slf4j.LoggerFactory

import com.anrisoftware.propertiesutils.ContextProperties
import com.anrisoftware.rsscon.api.NativeRssconInputStreamFactory
import com.anrisoftware.rsscon.api.NativeRssconOutputStreamFactory
import com.anrisoftware.rsscon.api.RssconNativeFactory
import com.anrisoftware.rsscon.utils.VirtualTtyEnvironment
import com.google.inject.Guice
import com.google.inject.Injector

/**
 * Creates the factories for the native rsscon.
 *
 * @author Erwin Mueller, erwin.mueller@deventm.org
 * @since 1.0
 */
class RssconTestUtils {

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

	VirtualTtyEnvironment environment

	@BeforeClass
	static void checkDeviceAvailable() {
		def log = LoggerFactory.getLogger(RssconTestUtils)
		def p = new ContextProperties(this, System.properties)
		deviceAvailable = p.getBooleanProperty("rsscon_available", false)
		devicePath = p.getProperty("rsscon_path", devicePaths[0])
		if (!deviceAvailable) {
			log.warn "Rsscon device is not available for testing."
		} else {
			log.debug "Rsscon device available for testing with path {}.", devicePaths
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

	@Before
	void startVirtualCom() {
		environment = new VirtualTtyEnvironment().startSocat()
	}

	@After
	void stopVirtualCom() {
		environment.stopSocat()
	}
}
