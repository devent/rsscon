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

/**
 * Enumarates the Rsscon error values.
 * 
 * @author Erwin Mueller, erwin.mueller@deventm.org
 * @since 1.0
 */
public enum RssconError {

	/**
	 * No error.
	 */
	NOERROR(0),

	/**
	 * Error opening the device.
	 */
	OPENDEVICE(-1),

	/**
	 * Error closing the device.
	 */
	CLOSEDEVICE(-2),

	/**
	 * Error setup the device.
	 */
	SETUPDEVICE(-3),

	/**
	 * Error setting flags because device is already open.
	 */
	DEVICE_OPENED(-4),

	/**
	 * Error writing to the device.
	 */
	ERROR_WRITE(-5),

	/**
	 * Error reading from the device.
	 */
	ERROR_READ(-6);

	/**
	 * Parse the specified error value to the rsscon error.
	 * 
	 * @param value
	 *            the error value.
	 * 
	 * @return the {@link RssconError}.
	 * 
	 * @throws IllegalArgumentException
	 *             if the specified error value is not a rsscon error.
	 */
	public static RssconError valueOf(int value) {
		for (RssconError error : RssconError.values()) {
			if (error.getValue() == value) {
				return error;
			}
		}
		throw new IllegalArgumentException();
	}

	private final int value;

	private RssconError(int value) {
		this.value = value;
	}

	/**
	 * Returns the error value.
	 * 
	 * @return the error value.
	 */
	public int getValue() {
		return value;
	}
}
