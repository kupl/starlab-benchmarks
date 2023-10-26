package org.apache.maven.shared.utils.xml;

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

import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

/**
 * 
 */
public class Xpp3DomUtils
{
    /**
     * @param dominant {@link Xpp3Dom}
     * @param recessive {@link Xpp3Dom}
     * @param childMergeOverride true/false.
     * @return Merged dom.
     */
    public static Xpp3Dom mergeXpp3Dom( Xpp3Dom dominant, Xpp3Dom recessive, Boolean childMergeOverride )
    {
        return dominant != null ? merge( dominant, recessive, childMergeOverride ) : recessive;
    }

    /**
     * @param dominant {@link Xpp3Dom}
     * @param recessive {@link Xpp3Dom}
     * @return Merged dom.
     */
    public static Xpp3Dom mergeXpp3Dom( Xpp3Dom dominant, Xpp3Dom recessive )
    {
        return dominant != null ? merge( dominant, recessive, null ) : recessive;
    }

    /**
     * @param dominant {@link Xpp3Dom}
     * @param recessive {@link Xpp3Dom}
     * @param childMergeOverride true/false.
     * @return Merged dom.
     */
/**
 *
 *
 * @param dominant
 * 		{@link Xpp3Dom}
 * @param recessive
 * 		{@link Xpp3Dom}
 * @param childMergeOverride
 * 		true/false.
 * @return Merged dom.
 */
public static org.apache.maven.shared.utils.xml.Xpp3Dom merge(org.apache.maven.shared.utils.xml.Xpp3Dom dominant, org.apache.maven.shared.utils.xml.Xpp3Dom recessive, java.lang.Boolean childMergeOverride) {
    if ((recessive == null) || org.apache.maven.shared.utils.xml.Xpp3DomUtils.isCombineSelfOverride(dominant)) {
        return dominant;
    }
    if (org.apache.maven.shared.utils.xml.Xpp3DomUtils.isEmpty(dominant.getValue())) {
        dominant.setValue(recessive.getValue());
    }
    for (java.lang.String attr : recessive.getAttributeNames()) {
        if (org.apache.maven.shared.utils.xml.Xpp3DomUtils.isEmpty(dominant.getAttribute(attr))) {
            dominant.setAttribute(attr, recessive.getAttribute(attr));
        }
    }
    if (recessive.getChildCount() > 0) {
        boolean mergeChildren = org.apache.maven.shared.utils.xml.Xpp3DomUtils.isMergeChildren(dominant, childMergeOverride);
        if (mergeChildren) {
            java.util.Map<java.lang.String, java.util.Iterator<org.apache.maven.shared.utils.xml.Xpp3Dom>> commonChildren = org.apache.maven.shared.utils.xml.Xpp3DomUtils.getCommonChildren(dominant, recessive);
            for (org.apache.maven.shared.utils.xml.Xpp3Dom recessiveChild : recessive) {
                java.util.Iterator<org.apache.maven.shared.utils.xml.Xpp3Dom> it = commonChildren.get(recessiveChild.getName());
                {
                    if (/* NPEX_NULL_EXP */
                    it.hasNext()) {
                        org.apache.maven.shared.utils.xml.Xpp3Dom dominantChild = it.next();
                        org.apache.maven.shared.utils.xml.Xpp3DomUtils.merge(dominantChild, recessiveChild, childMergeOverride);
                    }
                }
            }
        } else {
            org.apache.maven.shared.utils.xml.Xpp3Dom[] dominantChildren = dominant.getChildren();
            dominant.childList.clear();
            for (org.apache.maven.shared.utils.xml.Xpp3Dom child : recessive) {
                dominant.addChild(new org.apache.maven.shared.utils.xml.Xpp3Dom(child));
            }
            for (org.apache.maven.shared.utils.xml.Xpp3Dom aDominantChildren : dominantChildren) {
                dominant.addChild(aDominantChildren);
            }
        }
    }
    return dominant;
}

    private static Map<String, Iterator<Xpp3Dom>> getCommonChildren( Xpp3Dom dominant, Xpp3Dom recessive )
    {
        Map<String, Iterator<Xpp3Dom>> commonChildren = new HashMap<String, Iterator<Xpp3Dom>>();

        for ( String childName : recessive.childMap.keySet() )
        {
            List<Xpp3Dom> dominantChildren = dominant.getChildrenList( childName );
            if ( dominantChildren.size() > 0 )
            {
                commonChildren.put( childName, dominantChildren.iterator() );
            }
        }
        return commonChildren;
    }

    private static boolean isCombineSelfOverride( Xpp3Dom xpp3Dom )
    {
        String selfMergeMode = xpp3Dom.getAttribute( Xpp3Dom.SELF_COMBINATION_MODE_ATTRIBUTE );
        return Xpp3Dom.SELF_COMBINATION_OVERRIDE.equals( selfMergeMode );
    }

    private static boolean isMergeChildren( Xpp3Dom dominant, Boolean override )
    {
        return override != null ? override : !isMergeChildren( dominant );
    }

    private static boolean isMergeChildren( Xpp3Dom dominant )
    {
        return Xpp3Dom.CHILDREN_COMBINATION_APPEND.equals(
            dominant.getAttribute( Xpp3Dom.CHILDREN_COMBINATION_MODE_ATTRIBUTE ) );
    }

    /**
     * @param str The string to be checked.
     * @return <code>true</code> in case string is empty <code>false</code> otherwise.
     */
    public static boolean isEmpty( String str )
    {
        return str == null || str.trim().length() == 0;
    }




}
