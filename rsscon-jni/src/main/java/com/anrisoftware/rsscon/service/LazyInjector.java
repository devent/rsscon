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

import static com.google.inject.Guice.createInjector;

import org.apache.commons.lang3.concurrent.ConcurrentException;
import org.apache.commons.lang3.concurrent.LazyInitializer;

import com.anrisoftware.rsscon.nativeimpl.NativeRssconModule;
import com.google.inject.Injector;
import com.google.inject.Module;

/**
 * Lazy create the injector with the needed modules.
 * 
 * @author Erwin Mueller, erwin.mueller@deventm.org
 * @since 1.5
 */
class LazyInjector extends LazyInitializer<Injector> {

	private Injector parentInjector;

	/**
	 * Sets the optional parent injector.
	 * 
	 * @param injector
	 *            the parent {@link Injector}.
	 */
	public void setParentInjector(Injector injector) {
		parentInjector = injector;
	}

	@Override
	protected Injector initialize() throws ConcurrentException {
		Module[] modules = new Module[] { new NativeRssconModule() };
		if (parentInjector != null) {
			return parentInjector.createChildInjector(modules);
		}
		return createInjector(modules);
	}

}
