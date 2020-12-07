package br.com.caelum.stella.boleto.example;

import br.com.caelum.stella.boleto.Banco;
import br.com.caelum.stella.boleto.Boleto;
import br.com.caelum.stella.boleto.Datas;
import br.com.caelum.stella.boleto.Emissor;
import br.com.caelum.stella.boleto.Sacado;
import br.com.caelum.stella.boleto.bancos.BancoDoBrasil;
import br.com.caelum.stella.boleto.bancos.Bradesco;
import br.com.caelum.stella.boleto.transformer.BoletoGenerator;

public class BoletosToPdfExample {
    public static void main(String[] args) {
        BoletoGenerator gerador = new BoletoGenerator(getBoletoBancoDoBrasil(),
                getBoletoBradesco(), getBoletoBancoDoBrasil());

        // Para gerar um boleto em PDF
        gerador.toPDF("Boletos.pdf");
    }

    private static Boleto getBoletoBancoDoBrasil() {
        Datas datas = Datas.newDatas()
            .withDocumento(1, 5, 2008)
            .withProcessamento(1, 5, 2008)
            .withVencimento(2, 5, 2008);

        Emissor emissor = Emissor.newEmissor()
            .withCedente("Fulano de Tal")
            .withAgencia(2345)
            .withDigitoAgencia('6')
            .withContaCorrente(12345)
            .withNumeroConvenio(1234567)
            .withDigitoContaCorrente('1')
            .withCarteira(22)
            .withNossoNumero(9050987);

        Sacado sacado = Sacado.newSacado()
            .withNome("Fulano da Silva")
            .withCpf("111.222.333-12")
            .withEndereco("Av dos testes, 111 apto 333")
            .withBairro("Bairro Teste")
            .withCep("01234-111")
            .withCidade("São Paulo")
            .withUf("SP");

        Banco banco = new BancoDoBrasil();

        Boleto boleto = Boleto.newBoleto()
            .withBanco(banco)
            .withDatas(datas)
            .withDescricoes("descricao 1", "descricao 2", "descricao 3",
                        "descricao 4", "descricao 5")
            .withEmissor(emissor)
            .withSacado(sacado)
            .withValorBoleto("200.00")
            .withNumeroDoDocumento("1234")
            .withInstrucoes("instrucao 1", "instrucao 2",
                        "instrucao 3", "instrucao 4", "instrucao 5")
            .withLocaisDePagamento("local 1", "local 2")
            .withNumeroDoDocumento("4343");
        return boleto;
    }

    private static Boleto getBoletoBradesco() {
        Datas datas = Datas.newDatas()
            .withDocumento(1, 5, 2008)
            .withProcessamento(1, 5, 2008)
            .withVencimento(2, 5, 2008);

        Emissor emissor = Emissor.newEmissor()
            .withCedente("Fulano de Tal")
            .withAgencia(2345)
            .withDigitoAgencia('6')
            .withContaCorrente(12345)
            .withNumeroConvenio(1234567)
            .withDigitoContaCorrente('1')
            .withCarteira(22)
            .withNossoNumero(9050987);

        Sacado sacado = Sacado.newSacado()
            .withNome("Fulano da Silva")
            .withCpf("111.222.333-12")
            .withEndereco("Av dos testes, 111 apto 333")
            .withBairro("Bairro Teste")
            .withCep("01234-111")
            .withCidade("São Paulo")
            .withUf("SP");

        Banco banco = new Bradesco();

        Boleto boleto = Boleto.newBoleto()
            .withBanco(banco)
            .withDatas(datas)
            .withDescricoes("descricao 1", "descricao 2", "descricao 3",
                        "descricao 4", "descricao 5")
            .withEmissor(emissor)
            .withSacado(sacado)
            .withValorBoleto("200.00")
            .withNumeroDoDocumento("1234")
            .withInstrucoes("instrucao 1", "instrucao 2",
                        "instrucao 3", "instrucao 4", "instrucao 5")
            .withLocaisDePagamento("local 1", "local 2")
            .withNumeroDoDocumento("4343");
        return boleto;
    }
}
