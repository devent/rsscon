/*
 * Copyright 2012 Erwin Müller <erwin.mueller@deventm.org>
 *
 * This file is part of rsscon-api.
 *
 * rsscon-api is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * rsscon-api is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with rsscon-api. If not, see <http://www.gnu.org/licenses/>.
 */
package com.anrisoftware.rsscon.api;

/**
 * Makes the streams available that read and write data to or from a RS-232
 * (serial) device.
 * 
 * @author Erwin Mueller, erwin.mueller@deventm.org
 * @since 1.0
 */
public interface RssconService {

	/**
	 * Returns the information about this service.
	 * 
	 * @return the {@link RssconServiceInfo}.
	 */
	RssconServiceInfo getServiceInfo();

	/**
	 * Returns the factory to create the stream that reads data from a RS-232
	 * (serial) device.
	 * 
	 * @param <T>
	 *            the type of the factory.
	 * 
	 * @return a sub-type of {@link RssconInputStreamFactory}.
	 */
	<T extends RssconInputStreamFactory> T getInputStreamFactory();

	/**
	 * Returns the factory to create the stream that writes data to a RS-232
	 * (serial) device.
	 * 
	 * @param <T>
	 *            the type of the factory.
	 * 
	 * @return a sub-type of {@link RssconOutputStreamFactory}.
	 */
	<T extends RssconOutputStreamFactory> T getOutputStreamFactory();
}
