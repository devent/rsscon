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

import java.io.IOException;

import org.apache.commons.lang3.builder.ToStringBuilder;

import com.anrisoftware.rsscon.api.BaudRate;
import com.anrisoftware.rsscon.api.RssconNative;
import com.google.inject.Inject;
import com.google.inject.assistedinject.Assisted;

/**
 * The binding to the native rsscon driver library.
 * 
 * @author Erwin Mueller, erwin.mueller@deventm.org
 * @since 1.0
 */
class RssconNativeImpl extends NativeBinding implements RssconNative {

	private final RssconNativeImplLogger log;

	private final String device;

	private final BaudRate baudRate;

	private boolean open;

	private long reference;

	private boolean block;

	private boolean wait;

	@Inject
	RssconNativeImpl(RssconNativeImplLogger logger, @Assisted String device,
			@Assisted BaudRate baudRate) {
		this.log = logger;
		this.device = device;
		this.baudRate = baudRate;
		this.open = false;
	}

	@Override
	public final long getReference() {
		return reference;
	}

	@Override
	public final void setReference(long reference) {
		this.reference = reference;
	}

	@Override
	public void setBlocking(boolean block) throws IOException {
		log.blockingSet(this, block);
		this.block = block;
	}

	@Override
	public void setWait(boolean wait) throws IOException {
		log.blockingSet(this, wait);
		this.wait = wait;
	}

	public void close() throws IOException {
		if (!isOpen()) {
			return;
		}
		log.closeRsscon(this);
		rssconClose(reference);
		rssconFree(reference);
		open = false;
	}

	public void open() throws IOException {
		log.openDevice(this);
		long reference = rssconCreate(device, baudRate.getBoudRate());
		setReference(reference);
		rssconInit(reference);
		rssconSetBlocking(reference, block);
		rssconSetWait(reference, wait);
		rssconOpen(reference);
		open = true;
	}

	public boolean isOpen() throws IOException {
		if (reference == 0) {
			return false;
		} else {
			return open;
		}
	}

	public void write(byte[] data, int length) throws IOException {
		rssconWrite(reference, data, length);
		log.writeBuffer(this, data);
	}

	public int read(byte[] data, int length) throws IOException {
		int bytes = rssconRead(reference, data, length);
		log.readBuffer(this, data, bytes);
		return bytes;
	}

	@Override
	public String toString() {
		return new ToStringBuilder(this).append("device", device)
				.append("baud rate", baudRate).toString();
	}

}
