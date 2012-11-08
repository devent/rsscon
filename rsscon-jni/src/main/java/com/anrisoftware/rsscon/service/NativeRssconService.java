package com.anrisoftware.rsscon.service;

import org.apache.commons.lang3.concurrent.ConcurrentException;

import com.anrisoftware.rsscon.api.NativeRssconInputStreamFactory;
import com.anrisoftware.rsscon.api.NativeRssconOutputStreamFactory;
import com.anrisoftware.rsscon.api.RssconNative;
import com.anrisoftware.rsscon.api.RssconService;
import com.anrisoftware.rsscon.api.RssconServiceInfo;
import com.google.inject.Injector;

public class NativeRssconService implements RssconService {

	private static final String NAME = "native-rsscon";

	public static final RssconServiceInfo INFO = new RssconServiceInfo(NAME);

	private final LazyInjector lazyInjector;

	public NativeRssconService() {
		this.lazyInjector = new LazyInjector();
	}

	@Override
	public RssconServiceInfo getServiceInfo() {
		return INFO;
	}

	public void setParentInjector(Injector injector) {
		lazyInjector.setParentInjector(injector);
	}

	public RssconNative getRssconNative() {
		return getInjector().getInstance(RssconNative.class);
	}

	@SuppressWarnings("unchecked")
	@Override
	public NativeRssconInputStreamFactory getInputStreamFactory() {
		return getInjector().getInstance(NativeRssconInputStreamFactory.class);
	}

	@SuppressWarnings("unchecked")
	@Override
	public NativeRssconOutputStreamFactory getOutputStreamFactory() {
		return getInjector().getInstance(NativeRssconOutputStreamFactory.class);
	}

	private Injector getInjector() {
		try {
			return lazyInjector.get();
		} catch (ConcurrentException e) {
			throw new RuntimeException(e);
		}
	}
}
