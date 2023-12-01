<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ taglib prefix="fmt" uri="http://java.sun.com/jsp/jstl/fmt" %>

<html>
<head>
    <title>JSTL: Formatting/I18N Support -- Number, Currency, and Percent
        Example
    </title>
</head>
<body bgcolor="#FFFFFF">
<h3>Formatting numbers, currencies, and percentages using browser-based
    locale
</h3>

<c:catch var="ex">
    <ul>
        <li> Format &quot;123456789&quot; as number:<br>
                <fmt:formatNumber value="123456789"/>

        <li> Format &quot;123456789&quot; as percent:<br>
            <fmt:formatNumber type="percent">123456789</fmt:formatNumber>

        <li> Format &quot;12345.67&quot; as currency:<br>
                <fmt:formatNumber value="12345.67" type="currency"/>

        <li> Format &quot;12345.67&quot; as currency, with
            grouping turned off, the maximum number of digits in the integer portion
            limited to 4, and no fraction portion:<br>
                <fmt:formatNumber value="12345.67" type="currency"
                                  groupingUsed="false" maxIntegerDigits="4"
                                  maxFractionDigits="0"/>

        <li> Format &quot;12345.67&quot; as currency:<br>
                <fmt:formatNumber value="12345.67" type="currency"/><br>
            then parse its integer portion only and output the result:<br>
                <fmt:formatNumber value="12345.67" type="currency" var="cur"/>
                <fmt:parseNumber value="${cur}" type="currency" integerOnly="true"/>
    </ul>
</c:catch>

<c:if test="${not empty ex}">
    <font color="#FF0000">
        <br> The following error has occurred:<br><br>
        <c:out value="${ex}" escapeXml='false'/> <br>
        <c:if test="${ex.rootCause.class.name == 'java.text.ParseException' && pageContext.response.locale == 'de_DE'}">
            <br> This is due to a known bug in java.text.NumberFormat (Bugtraq bugid: 4709840).
        </c:if>
    </font>
</c:if>

</body>
</html>
