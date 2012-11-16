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

import static com.google.inject.Guice.createInjector;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URL;
import java.util.List;
import java.util.Properties;

import com.anrisoftware.nativeloader.api.NativeLoader;
import com.anrisoftware.nativeloader.api.NativeLoaderFactory;
import com.anrisoftware.nativeloader.service.NativeLoaderModule;
import com.anrisoftware.propertiesutils.ContextProperties;
import com.anrisoftware.propertiesutils.ContextPropertiesFactory;
import com.anrisoftware.rsscon.api.NativeRssconInputStreamFactory;
import com.anrisoftware.rsscon.api.NativeRssconOutputStreamFactory;
import com.anrisoftware.rsscon.api.RssconNative;
import com.anrisoftware.rsscon.api.RssconNativeFactory;
import com.google.inject.AbstractModule;
import com.google.inject.assistedinject.FactoryModuleBuilder;

/**
 * Installs the native factories.
 * 
 * @author Erwin Mueller, erwin.mueller@deventm.org
 * @since 1.0
 */
public class NativeRssconModule extends AbstractModule {

	private static final URL NATIVE_LIBS_PROPERTIES_URL = NativeRssconModule.class
			.getResource("native_libs.properties");

	@Override
	protected void configure() {
		install(new FactoryModuleBuilder().implement(RssconNative.class,
				RssconNativeImpl.class).build(RssconNativeFactory.class));
		install(new FactoryModuleBuilder().implement(OutputStream.class,
				RssconOutputStream.class).build(
				NativeRssconOutputStreamFactory.class));
		install(new FactoryModuleBuilder().implement(InputStream.class,
				RssconInputStream.class).build(
				NativeRssconInputStreamFactory.class));
		loadLibraries();
	}

	private void loadLibraries() {
		ContextProperties p = new ContextProperties(this,
				getNativeLibrariesProperties());
		NativeLoaderFactory factory = createInjector(new NativeLoaderModule())
				.getInstance(NativeLoaderFactory.class);
		List<String> libs = p.getListProperty("libs");
		NativeLoader loader = factory.create(libs.toArray(new String[0]));
		loader.loadLibrary();
	}

	Properties getNativeLibrariesProperties() {
		try {
			return new ContextPropertiesFactory(this).withProperties(
					System.getProperties()).fromResource(
					NATIVE_LIBS_PROPERTIES_URL);
		} catch (IOException e) {
			throw new RuntimeException(e);
		}
	}
}
