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
package com.anrisoftware.rsscon.api;

import java.io.IOException;

/**
 * Manage the reference to the native device library.
 * 
 * @author Erwin Mueller, erwin.mueller@deventm.org
 * @since 1.0
 */
public interface RssconNative {

	/**
	 * Returns the reference to the device.
	 * 
	 * @return the reference.
	 */
	long getReference();

	/**
	 * Sets the reference to the device.
	 * 
	 * @param reference
	 *            the reference.
	 */
	void setReference(long reference);

	/**
	 * Sets whether the read and write access should block. Defaults to false.
	 * 
	 * @param block
	 *            set to {@code true} to block, {@code false} to not block.
	 * 
	 * @throws IOException
	 *             if there was error set blocking.
	 */
	void setBlocking(boolean block) throws IOException;

	/**
	 * Sets whether to wait for the device. Defaults to false.
	 * 
	 * @param wait
	 *            set to {@code true} to wait, {@code false} to not wait.
	 * 
	 * @throws IOException
	 *             if there was error set wait.
	 */
	void setWait(boolean wait) throws IOException;
}
