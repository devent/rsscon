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
package com.anrisoftware.rsscon.service;

import org.apache.commons.lang3.concurrent.ConcurrentException;
import org.mangosdk.spi.ProviderFor;

import com.anrisoftware.rsscon.api.NativeRssconInputStreamFactory;
import com.anrisoftware.rsscon.api.NativeRssconOutputStreamFactory;
import com.anrisoftware.rsscon.api.RssconNativeFactory;
import com.anrisoftware.rsscon.api.RssconService;
import com.anrisoftware.rsscon.api.RssconServiceInfo;
import com.google.inject.Injector;

/**
 * Native Rsscon service.
 * 
 * @author Erwin Mueller, erwin.mueller@deventm.org
 * @since 1.0
 */
@ProviderFor(RssconService.class)
public class NativeRssconService implements RssconService {

	private static final String NAME = "native-rsscon";

	/**
	 * The {@link RssconServiceInfo} information about the native Rsscon
	 * service.
	 */
	public static final RssconServiceInfo INFO = new RssconServiceInfo(NAME);

	private final LazyInjector lazyInjector;

	/**
	 * Creates the lazy injector.
	 */
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

	public RssconNativeFactory getRssconNative() {
		return getInjector().getInstance(RssconNativeFactory.class);
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
