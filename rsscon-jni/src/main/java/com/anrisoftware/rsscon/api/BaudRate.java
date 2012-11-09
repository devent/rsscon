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
 * Defines the valid baud rates for the device. The valid baud rates are:
 * <ul>
 * <li>57600</li>
 * <li>115200</li>
 * <li>230400</li>
 * <li>460800</li>
 * <li>500000</li>
 * <li>576000</li>
 * <li>921600</li>
 * <li>1000000</li>
 * <li>1152000</li>
 * <li>1500000</li>
 * <li>2000000</li>
 * <li>2500000</li>
 * <li>3000000</li>
 * <li>3500000</li>
 * <li>4000000</li>
 * </ul>
 * 
 * @author Erwin Mueller, erwin.mueller@deventm.org
 * @since 1.0
 */
public enum BaudRate {

	BAUDRATE_57600(57600), BAUDRATE_115200(115200), BAUDRATE_230400(230400), BAUDRATE_460800(
			460800), BAUDRATE_500000(500000), BAUDRATE_576000(576000), BAUDRATE_921600(
			921600), BAUDRATE_1000000(1000000), BAUDRATE_1152000(1152000), BAUDRATE_1500000(
			1500000), BAUDRATE_2000000(2000000), BAUDRATE_2500000(2500000), BAUDRATE_3000000(
			3000000), BAUDRATE_3500000(3500000), BAUDRATE_4000000(4000000);

	private int boudRate;

	private BaudRate(int boudRate) {
		this.boudRate = boudRate;
	}

	/**
	 * Get the integer value of the boud rate.
	 * 
	 * @return the integer value.
	 */
	public int getBoudRate() {
		return boudRate;
	}
}
