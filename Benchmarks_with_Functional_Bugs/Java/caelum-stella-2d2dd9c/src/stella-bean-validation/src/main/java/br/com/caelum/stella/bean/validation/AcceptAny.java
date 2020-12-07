package br.com.caelum.stella.bean.validation;

import static java.lang.annotation.ElementType.FIELD;
import static java.lang.annotation.ElementType.METHOD;

import java.lang.annotation.Documented;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

import javax.validation.Constraint;
import javax.validation.Payload;

import br.com.caelum.stella.bean.validation.logic.StellaAcceptAnyValidator;
import br.com.caelum.stella.validation.AcceptAnyValidator.Documento;

/**
 * Restrição que pode ser associada a objetos em que o método
 * {@linkplain #toString()} represente um dos documentos passados como
 * parametro.
 * 
 * @author Leonardo Bessa
 * @author David Paniz
 */
@Documented
@Retention(RetentionPolicy.RUNTIME)
@Target({ FIELD, METHOD })
@Constraint(validatedBy = StellaAcceptAnyValidator.class)
public @interface AcceptAny {
	String message() default "{br.com.caelum.stella.bean.validation.AcceptAny.message}";

	boolean formatted() default false;

	Documento[] documentos();
	
    Class<?>[] groups() default {};

    Class<? extends Payload>[] payload() default {};

}
