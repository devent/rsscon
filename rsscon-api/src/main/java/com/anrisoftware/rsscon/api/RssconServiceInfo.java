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

import org.apache.commons.lang3.builder.HashCodeBuilder;

/**
 * Information for the Rsscon streams service.
 * 
 * @author Erwin Mueller, erwin.mueller@deventm.org
 * @since 1.0
 */
public class RssconServiceInfo {

	private final String name;

	/**
	 * Sets the name of the service.
	 * 
	 * @param name
	 *            the name.
	 */
	public RssconServiceInfo(String name) {
		this.name = name;
	}

	/**
	 * Returns the name of the service.
	 * 
	 * @return the name.
	 */
	public String getName() {
		return name;
	}

	@Override
	public String toString() {
		return name;
	}

	/**
	 * Compare this information to the specified service name or the to
	 * specified service information.
	 */
	@Override
	public boolean equals(Object obj) {
		if (obj == null) {
			return false;
		}
		if (obj == this) {
			return true;
		}
		if (obj instanceof String) {
			String name = (String) obj;
			return getName().equals(name);
		}
		if (!(obj instanceof RssconServiceInfo)) {
			return false;
		}
		RssconServiceInfo rhs = (RssconServiceInfo) obj;
		return getName().equals(rhs.getName());
	}

	@Override
	public int hashCode() {
		return new HashCodeBuilder().append(getName()).toHashCode();
	}
}
