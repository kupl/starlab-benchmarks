package br.com.caelum.stella.bean.validation;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import javax.validation.ConstraintValidatorContext;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mockito;

import br.com.caelum.stella.bean.validation.logic.StellaCPFValidator;

/**
 * @author Fabio Kung
 * @author David Paniz
 */
public class StellaCPFValidatorTest {
	private ConstraintValidatorContext context = Mockito.mock(ConstraintValidatorContext.class);
	private StellaCPFValidator validator;

	private static class ObjectWithCPF {
		@CPF
		private String cpf;
	}
	
	private static class ObjectWithCPFConfigured {
		@CPF(ignoreRepeated=true)
		private String cpf;
	}

	@Before
	public void createValidator() throws Exception {
		CPF cpfAnnotation = ObjectWithCPF.class.getDeclaredField("cpf").getAnnotation(CPF.class);
		validator = new StellaCPFValidator();
		validator.initialize(cpfAnnotation);
	}
	
	@Test
	public void shouldNotValidateCPFWithRepeatedDigitsByDefault() {
		boolean valid = validator.isValid("00000000000", context);
		assertFalse(valid);
	}
	
	@Test
	public void shouldValidateCPFWithRepeatedDigitsIfConfigured() throws Exception{
		CPF cpfAnnotation = ObjectWithCPFConfigured.class.getDeclaredField("cpf").getAnnotation(CPF.class);
		StellaCPFValidator anotherValidator = new StellaCPFValidator();
		anotherValidator.initialize(cpfAnnotation);
		
		boolean valid = anotherValidator.isValid("00000000000", context);
		assertTrue(valid);
	}

	@Test
	public void shouldOnlyValidateStrings() {
		boolean valid = validator.isValid("03118383402", context);
		assertTrue(valid);
	}

	@Test
	public void shouldValidateNull() {
		boolean valid = validator.isValid(null, context);
		assertTrue(valid);
	}

	@Test
	public void shouldValidateEmpty() {
		boolean valid = validator.isValid("", context);
		assertTrue(valid);
	}
}
