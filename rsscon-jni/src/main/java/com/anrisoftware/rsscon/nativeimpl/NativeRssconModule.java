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

import java.io.InputStream;
import java.io.OutputStream;

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
	}

}
