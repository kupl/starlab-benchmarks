/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.apache.taglibs.standard.examples.beans;

/**
 * Object that represents a Customer.
 *
 * @author Pierre Delisle
 */

public class Address {

    //*********************************************************************
    // Instance variables

    /**
     * Holds value of property line1.
     */
    private String line1;

    /**
     * Holds value of property line2.
     */
    private String line2;

    /**
     * Holds value of property city.
     */
    private String city;

    /**
     * Holds value of property zip.
     */
    private String zip;

    /**
     * Holds value of property state.
     */
    private String state;

    /**
     * Holds value of property country.
     */
    private String country;

    //*********************************************************************
    // Constructor

    public Address(String line1, String line2, String city,
                   String state, String zip, String country) {
        setLine1(line1);
        setLine2(line2);
        setCity(city);
        setState(state);
        setZip(zip);
        setCountry(country);
    }

    //*********************************************************************
    // Accessors

    /**
     * Getter for property line1.
     *
     * @return Value of property line1.
     */
    public String getLine1() {
        return line1;
    }

    /**
     * Setter for property line1.
     *
     * @param line1 New value of property line1.
     */
    public void setLine1(String line1) {
        this.line1 = line1;
    }

    /**
     * Getter for property line2.
     *
     * @return Value of property line2.
     */
    public String getLine2() {
        return line2;
    }

    /**
     * Setter for property line2.
     *
     * @param line2 New value of property line2.
     */
    public void setLine2(String line2) {
        this.line2 = line2;
    }

    /**
     * Getter for property city.
     *
     * @return Value of property city.
     */
    public String getCity() {
        return city;
    }

    /**
     * Setter for property city.
     *
     * @param city New value of property city.
     */
    public void setCity(String city) {
        this.city = city;
    }

    /**
     * Getter for property zip.
     *
     * @return Value of property zip.
     */
    public String getZip() {
        return zip;
    }

    /**
     * Setter for property zip.
     *
     * @param zip New value of property zip.
     */
    public void setZip(String zip) {
        this.zip = zip;
    }

    /**
     * Getter for property country.
     *
     * @return Value of property country.
     */
    public String getCountry() {
        return country;
    }

    /**
     * Setter for property country.
     *
     * @param country New value of property country.
     */
    public void setCountry(String country) {
        this.country = country;
    }

    /**
     * Getter for property state.
     *
     * @return Value of property state.
     */
    public String getState() {
        return state;
    }

    /**
     * Setter for property state.
     *
     * @param state New value of property state.
     */
    public void setState(String state) {
        this.state = state;
    }

    //*********************************************************************
    // Utility Methods

    /**
     * Return a String representation of this object.
     */
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append(line1).append(" ");
        sb.append(city).append(" ");
        sb.append(country);
        return (sb.toString());
    }
}
