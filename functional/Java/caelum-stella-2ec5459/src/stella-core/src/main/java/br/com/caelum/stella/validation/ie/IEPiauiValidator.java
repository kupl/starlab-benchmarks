package br.com.caelum.stella.validation.ie;

import java.util.regex.Pattern;

import br.com.caelum.stella.DigitoPara;
import br.com.caelum.stella.MessageProducer;
import br.com.caelum.stella.SimpleMessageProducer;

/**
 * <p> Documentação de referência: </p>
 * <a href="http://www.sintegra.gov.br/Cad_Estados/cad_PI.html">
 * SINTEGRA - ROTEIRO DE CRÍTICA DA INSCRIÇÃO ESTADUAL </a>
 * 
 * @author Leonardo Bessa
 */
public class IEPiauiValidator extends AbstractIEValidator {

    public static final Pattern FORMATED = Pattern.compile("\\d{2}(\\.\\d{3}){2}\\-\\d{1}");

    public static final Pattern UNFORMATED = Pattern.compile("\\d{9}");

	
    /**
	 * Este considera, por padrão, que as cadeias estão formatadas e utiliza um
	 * {@linkplain SimpleMessageProducer} para geração de mensagens.
	 */
	public IEPiauiValidator() {
		super(true);
	}

	/**
	 * O validador utiliza um {@linkplain SimpleMessageProducer} para geração de
	 * mensagens.
	 * 
	 * @param isFormatted
	 *            considerar cadeia formatada quando <code>true</code>
	 */
	public IEPiauiValidator(boolean isFormatted) {
		super(isFormatted);
	}

	public IEPiauiValidator(MessageProducer messageProducer, boolean isFormatted) {
		super(messageProducer, isFormatted);
	}


	@Override
	protected Pattern getUnformattedPattern() {
		return UNFORMATED;
	}

	@Override
	protected Pattern getFormattedPattern() {
		return FORMATED;
	}

    protected boolean hasValidCheckDigits(String unformattedIE) {
		String iESemDigito = unformattedIE.substring(0, unformattedIE.length() - 1);
		String digito = unformattedIE.substring(unformattedIE.length() - 1);

		String digitoCalculado = calculaDigito(iESemDigito);

		return digito.equals(digitoCalculado);
    }

	private String calculaDigito(String iESemDigito) {
		DigitoPara digitoPara = new DigitoPara(iESemDigito);
		digitoPara.complementarAoModulo().trocandoPorSeEncontrar("0", 10, 11);

		return digitoPara.calcula();
	}

}
