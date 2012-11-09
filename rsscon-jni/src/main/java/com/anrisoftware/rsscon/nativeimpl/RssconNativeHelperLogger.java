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

import java.io.ByteArrayOutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;

import com.anrisoftware.globalpom.log.AbstractLogger;

/**
 * Logging messages for {@link RssconInputStream}.
 * 
 * @author Erwin Mueller, erwin.mueller@deventm.org
 * @since 1.0
 */
class RssconNativeHelperLogger extends AbstractLogger {

	/**
	 * Create logger for {@link RssconInputStream}.
	 */
	RssconNativeHelperLogger() {
		super(RssconInputStream.class);
	}

	void closeRsscon(RssconNativeHelper rsscon) {
		log.trace("Close {}.", rsscon);
	}

	void openDevice(RssconNativeHelper rsscon) {
		log.trace("Open {}.", rsscon);
	}

	void writeBuffer(RssconNativeHelper rsscon, byte[] data) {
		if (log.isTraceEnabled()) {
			log.trace("Write buffer {} to {}.", toHexString(data), rsscon);
		}
	}

	private String toHexString(byte[] data) {
		return toHexString(data, data.length);
	}

	void readBuffer(RssconNativeHelper rsscon, byte[] data, int bytes) {
		if (log.isTraceEnabled()) {
			log.trace("Write buffer {} to {}.", toHexString(data, bytes),
					rsscon);
		}
	}

	private String toHexString(byte[] array, int bytes) {
		ByteArrayOutputStream out = new ByteArrayOutputStream();
		PrintWriter writer = new PrintWriter(new OutputStreamWriter(out));
		for (int i = 0; i < bytes; i++) {
			writer.printf("%2x", array[i]);
		}
		return out.toString();
	}

}
