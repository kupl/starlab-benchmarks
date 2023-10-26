package org.apache.maven.plugins.enforcer;

/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

import org.apache.commons.codec.digest.DigestUtils;
import org.apache.maven.enforcer.rule.api.EnforcerRuleException;
import org.apache.maven.enforcer.rule.api.EnforcerRuleHelper;

/**
 * Rule to validate a file to match the specified checksum.
 *
 * @author Edward Samson
 * @author Lyubomyr Shaydariv
 */
public class RequireFileChecksum
    extends AbstractNonCacheableEnforcerRule
{

    private File file;

    private String checksum;

    private String type;

    private String nonexistentFileMessage;

    @Override
public void execute(org.apache.maven.enforcer.rule.api.EnforcerRuleHelper helper) throws org.apache.maven.enforcer.rule.api.EnforcerRuleException {
    if (this.file == null) {
        throw new org.apache.maven.enforcer.rule.api.EnforcerRuleException("Input file unspecified");
    }
    {
        if (this.checksum == null) {
            throw new org.apache.maven.enforcer.rule.api.EnforcerRuleException("Checksum unspecified");
        }
        if (!this.file.exists()) {
            java.lang.String message = nonexistentFileMessage;
            if (message == null) {
                message = "File does not exist: " + this.file.getAbsolutePath();
            }
            throw new org.apache.maven.enforcer.rule.api.EnforcerRuleException(message);
        }
        if (this.file.isDirectory()) {
            throw new org.apache.maven.enforcer.rule.api.EnforcerRuleException("Cannot calculate the checksum of directory: " + this.file.getAbsolutePath());
        }
        if (!this.file.canRead()) {
            throw new org.apache.maven.enforcer.rule.api.EnforcerRuleException("Cannot read file: " + this.file.getAbsolutePath());
        }
        java.lang.String checksum = calculateChecksum();
        if (!checksum.equalsIgnoreCase(this.checksum)) {
            java.lang.String exceptionMessage = getMessage();
            if (exceptionMessage == null) {
                exceptionMessage = (((((/* NPEX_NULL_EXP */
                this.type + " hash of ") + this.file) + " was ") + checksum) + " but expected ") + this.checksum;
            }
            throw new org.apache.maven.enforcer.rule.api.EnforcerRuleException(exceptionMessage);
        }
    }
}

    /**
     * The file to check.
     *
     * @param file file
     */
    public void setFile( File file )
    {
        this.file = file;
    }

    /**
     * The expected checksum value.
     *
     * @param checksum checksum
     */
    public void setChecksum( String checksum )
    {
        this.checksum = checksum;
    }

    /**
     * The checksum algorithm to use. Possible values: "md5", "sha1", "sha256", "sha384", "sha512".
     *
     * @param type algorithm
     */
    public void setType( String type )
    {
        this.type = type;
    }

    /**
     * The friendly message to use when the file does not exist.
     *
     * @param nonexistentFileMessage message
     */
    public void setNonexistentFileMessage( String nonexistentFileMessage )
    {
        this.nonexistentFileMessage = nonexistentFileMessage;
    }

    private String calculateChecksum()
        throws EnforcerRuleException
    {
        try ( InputStream inputStream = new FileInputStream( this.file ) )
        {
            String checksum;
            if ( "md5".equals( this.type ) )
            {
                checksum = DigestUtils.md5Hex( inputStream );
            }
            else if ( "sha1".equals( this.type ) )
            {
                checksum = DigestUtils.shaHex( inputStream );
            }
            else if ( "sha256".equals( this.type ) )
            {
                checksum = DigestUtils.sha256Hex( inputStream );
            }
            else if ( "sha384".equals( this.type ) )
            {
                checksum = DigestUtils.sha384Hex( inputStream );
            }
            else if ( "sha512".equals( this.type ) )
            {
                checksum = DigestUtils.sha512Hex( inputStream );
            }
            else
            {
                throw new EnforcerRuleException( "Unsupported hash type: " + this.type );
            }
            return checksum;
        }
        catch ( IOException e )
        {
            throw new EnforcerRuleException( "Unable to calculate checksum", e );
        }
    }
}
