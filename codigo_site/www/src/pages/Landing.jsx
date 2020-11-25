import React, { Component } from 'react';

import '../styles/pages/landing.css';
import LogoImg from '../images/logo.jpeg';

class Landing extends Component {
  constructor(props) {
    super(props)
    this.state = {
      codigo: '',
      codigo_de_recuperacao: ''
    }
    this.ChangeCodigo = this.ChangeCodigo.bind(this)
    this.GerarSenha = this.GerarSenha.bind(this)
    this.somenteNumeros = this.somenteNumeros.bind(this)

  }
  ChangeCodigo(codigo) {
    this.setState({ codigo: codigo })
  }
  GerarSenha(e) {
    e.preventDefault();
    if (this.state.codigo !== '') {
      var number_array = this.state.codigo.split("");
      var numero_1 = parseInt(number_array[0]);
      var numero_2 = parseInt(number_array[1])||0;
      var numero_3 = parseInt(number_array[2])||0;
      var numero_4 = parseInt(number_array[3])||0;
      var numero_5 = numero_4 + numero_3;
      this.setState({ codigo_de_recuperacao: `${numero_5}${numero_1}${numero_4}${numero_5}${numero_3}${numero_2}${numero_5}` })
    } else{
      alert("Formato indevido")
    }
  }
  somenteNumeros(digito) {
    var expressao = /[^0-9.]/;
    var campo = digito;
    if (expressao.test(campo.target.value)) {
      alert("Precione apenas teclas numéricas")
      campo.target.value = '';
      this.setState({ codigo: '' })
    }
  }
  render() {
    return (
      <div id="page-landing">
        <div className="content-wrapper">
          <img src={LogoImg} className="imageLogo" alt="ControlLock-Logo" />
          <main>
            <div className="title">
              <h1>SUA CASA MAIS SEGURA</h1>
            </div>
          </main>
          <div className="title_mobile">
              <h1>Control Lock</h1>
            </div>
          <div className="location">
            <strong>Joinville</strong>
            <span> Santa Catarina</span>
          </div>
          <div className="BlocoInput">

            <form onSubmit={this.GerarSenha}>
              <div className="BlocoPin">
                <div className="BlocoPinTitle">
                  <label>PIN</label>
                </div>

                <div className="BlocoPinInput">
                  <input className="input" type="text" placeholder="PIN de 4 Numeros" onKeyUp={this.somenteNumeros} value={this.state.codigo} required pattern="\d{4}" maxLength="4"  onChange={e => this.ChangeCodigo(e.target.value)} />
                </div>

              </div>
              <button className="BotaoGerarPin" type="submit">Gerar</button>
            </form>
            
            <div className="BlocoCodigoRecuperacao">
              <div className="BlocoCodigoRecuperacaoTitle">
                <label>CODIGO DE RECUPERAÇÃO</label>
              </div>
              <div className="BlocoCodigoRecuperacaoInput">
                <input className="input" type="text" placeholder="Aperte em Gerar" defaultValue={this.state.codigo_de_recuperacao} autoComplete="off" maxLength="0" disabled="disabled" />
              </div>
            </div>
          </div>
          
        </div>
      </div>
    );
  }
}
export default Landing;
