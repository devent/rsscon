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

import static java.lang.String.format;

import java.io.IOException;

/**
 * I/O exception that contains the error number of the Rsscon device.
 * 
 * @author Erwin Mueller, erwin.mueller@deventm.org
 * @since 1.0
 */
public class RssconIOException extends IOException {

	/**
	 * @since 1.0
	 */
	private static final long serialVersionUID = 6004500446911818438L;

	private final RssconError lastError;

	private final int errorNumber;

	private final String error;

	/**
	 * Sets the message and the cause for the exception.
	 * 
	 * @param format
	 *            the format of the message. The device, last error, error
	 *            number and error are passed as the arguments for the format.
	 *            The format should have the following argument list:
	 *            {@code "%s %s %d %s"}.
	 * 
	 * @param device
	 *            the device path.
	 * 
	 * @param lastError
	 *            the last rsscon error. Should be the value of
	 *            {@link RssconError}.
	 * 
	 * @param errorNumber
	 *            the system specific error number.
	 * 
	 * @param error
	 *            the error string of the specific error number.
	 * 
	 * @param cause
	 *            the {@link Throwable} cause.
	 */
	public RssconIOException(String format, String device, int lastError,
			int errorNumber, String error, Throwable cause) {
		super(format(format, device, lastError, errorNumber, error), cause);
		this.lastError = RssconError.valueOf(lastError);
		this.errorNumber = errorNumber;
		this.error = error;
	}

	/**
	 * Sets the message for the exception.
	 * 
	 * @param format
	 *            the format of the message. The device, last error, error
	 *            number and error are passed as the arguments for the format.
	 *            The format should have the following argument list:
	 *            {@code "%s %s %d %s"}.
	 * 
	 * @param device
	 *            the device path.
	 * 
	 * @param lastError
	 *            the last rsscon error. Should be the value of
	 *            {@link RssconError}.
	 * 
	 * @param errorNumber
	 *            the system specific error number.
	 * 
	 * @param error
	 *            the error string of the specific error number.
	 */
	public RssconIOException(String message, String device, int lastError,
			int errorNumber, String error) {
		super(format(message, device, RssconError.valueOf(lastError),
				errorNumber, error));
		this.lastError = RssconError.valueOf(lastError);
		this.errorNumber = errorNumber;
		this.error = error;
	}

	/**
	 * Returns the rsscon error.
	 * 
	 * @return the {@link RssconError}.
	 */
	public RssconError getLastError() {
		return lastError;
	}

	/**
	 * Returns the system specific error.
	 * 
	 * @return the system specific error.
	 */
	public int getErrorNumber() {
		return errorNumber;
	}

	/**
	 * Returns the string of the system specific error.
	 * 
	 * @return the string of the system specific error.
	 */
	public String getError() {
		return error;
	}
}
