package com.anrisoftware.rsscon.nativeimpl

import org.junit.Before
import org.junit.BeforeClass

import com.anrisoftware.rsscon.api.NativeRssconInputStreamFactory
import com.anrisoftware.rsscon.api.NativeRssconOutputStreamFactory
import com.anrisoftware.rsscon.api.RssconNativeFactory
import com.anrisoftware.rsscon.nativeimpl.NativeRssconModule;
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

	static Injector injector

	static NativeRssconInputStreamFactory inputFactory

	static NativeRssconOutputStreamFactory outputFactory

	static RssconNativeFactory nativeFactory

	VirtualTtyEnvironment environment

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
}
