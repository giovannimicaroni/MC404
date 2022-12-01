int read(int __fd, const void *__buf, int __n){
  int bytes;
  __asm__ __volatile__(
    "mv a0, %1           # file descriptor\n"
    "mv a1, %2           # buffer \n"
    "mv a2, %3           # size \n"
    "li a7, 63           # syscall read (63) \n"
    "ecall \n"
    "mv %0, a0"
    : "=r"(bytes)  // Output list
    :"r"(__fd), "r"(__buf), "r"(__n)    // Input list
    : "a0", "a1", "a2", "a7"
  );
  return bytes;
}
 
void write(int __fd, const void *__buf, int __n){
  __asm__ __volatile__(
    "mv a0, %0           # file descriptor\n"
    "mv a1, %1           # buffer \n"
    "mv a2, %2           # size \n"
    "li a7, 64           # syscall write (64) \n"
    "ecall"
    :   // Output list
    :"r"(__fd), "r"(__buf), "r"(__n)    // Input list
    : "a0", "a1", "a2", "a7"
  );
}
 

int elevado(int base, int vezes){
  int resultado = 1;
  for(int i = 0; i < vezes; i++){
    resultado *= base;
  }
  return resultado;
}

int char_para_int(char *numero, int n, int base){
  ///dado uma string que representa um numero, devolve o inteiro em decimal do numero representado. Espera-se que o número dado seja válido (base <= 10).
 
  int i, resultado = 0;
  for(i = n; i > 0; i--){
    resultado += (numero[i-1] - '0')*elevado(base, n-i);
    }
    return resultado;
}

int stringlen(char *string){
  char atual = string[0];
  int tamanho = 0;
  while(atual != '\0'){
    tamanho++;
    atual = string[tamanho];
  }
  return tamanho;
}

void invertestring(char *string, char *stringinvertida){
  int tamanho = stringlen(string);
  for(int i = 0; i < tamanho; i++){
    stringinvertida[i] = string[tamanho-i-1];
  }
}

void complemento1(char *string, char *resultado){
  int tamanho = stringlen(string)-1;
  for(int i = 0; i <= tamanho; i++){
    if(string[i] == '1')
      resultado[i] = '0';
    else if(string[i] == '0')
      resultado[i] = '1';
    if(i == tamanho){
      resultado[i++] = '\n';
      resultado[i] = '\0';
    }
  }

}

void soma_um_binario(char *string, char *resultado){
  int carry = 1, tamanho = stringlen(string)-2;
  for(int j = tamanho; j >= 0; j--){
    if(string[j] == '0' && carry == 1){
      resultado[j] = '1';
      carry = 0;
    }
    else if(string[j] == '1' && carry == 1){
      resultado[j] = '0';
    }
    else{
      resultado[j] = string[j];
    }
  }
  resultado[tamanho+1] = '\n';
  resultado[tamanho+2] = '\0';
}


int soma_quarteto_binario(char digito1, char digito2, char digito3, char digito4){
  return (digito1 - '0')*8 + (digito2 - '0')*4 + (digito3 - '0')*2 + (digito4 - '0');
}

void binhexa(char* bin, char *resultado){
  int i, j=0, juntou = 0, valor_quarteto, n = stringlen(bin)-2; 
  char digito_atual;
  char hexainvertido[32];
  for(i = n; i >= 0; i--){
    juntou++;
    if(juntou == 4){
      valor_quarteto = soma_quarteto_binario(bin[i], bin[i+1], bin[i+2], bin[i+3]);
      switch (valor_quarteto){
        case 10:
          digito_atual = 'a';
          break;
        case 11:
          digito_atual = 'b';
          break;
        case 12:
          digito_atual = 'c';
          break;
        case 13:
          digito_atual = 'd';
          break;
        case 14:
          digito_atual = 'e';
          break;
        case 15:
          digito_atual = 'f';
          break;
        default:
          digito_atual = valor_quarteto + '0';
      }
      hexainvertido[j] = digito_atual;
      juntou = 0;
      j++;
      if(i == 0)
        hexainvertido[j] = '\0';
  }
  else if(i == 0 && juntou < 4){
    digito_atual = '0';
    for(int k = 0; k < juntou; k++){
      digito_atual += (bin[k] - '0')*elevado(2, juntou - k - 1);
    }
    hexainvertido[j] = digito_atual;
    
  }
  }
  invertestring(hexainvertido, resultado);
  resultado[j] = '\n';
  resultado[j+1] = '\0';
}

void poeprefixo(char p, char *s_numero, char *s_prefixada){
  int n = stringlen(s_numero);
  s_prefixada[0] = '0';
  s_prefixada[1] = p;
  for(int i = 0; i <= n; i++){
    s_prefixada[i+2] = s_numero[i];
  } 
}

void poemenos(char *numero, char *numeron){
  int n = stringlen(numero);
  numeron[0] = '-';
  for(int i = 0; i <= n; i++){
    numeron[i+1] = numero[i];
  }
}

void completacom0(char *binario, char *resultado){
  int tamanho = stringlen(binario)-2;
  int j = 0;
  for(int i = 0; i < 32; i++){
    if(32-i-1 > tamanho){
      resultado[i] = '0';
    }
    else{
      resultado[i] = binario[j++];
    }
  }
  resultado[32] = '\n';
  resultado[33] = '\0';
}

void decimalbinario(unsigned long int numero, char *binario){
  int i = 0,resto;
  char binarioinvertido[32];
  while(numero >= 1){
    resto = numero % 2;
    numero /= 2;
    binarioinvertido[i] = (resto+'0');
    i++;
  }
  binarioinvertido[i] = '\0';
  invertestring(binarioinvertido, binario);
  binario[i] = '\n';
  binario[i+1] = '\0';
}

unsigned long int hexatodecimal(char *numero){
  unsigned long int decimal = 0, soma_atual, n = stringlen(numero)-2;
  for(int i = 0; i <= n; i++){
    switch(numero[i]){
      case 'a':
        soma_atual = 10;
        break;
      case 'b':
        soma_atual = 11;
        break;
      case 'c':
        soma_atual = 12;
        break;
      case 'd':
        soma_atual = 13;
        break;
      case 'e':
        soma_atual = 14;
        break;
      case 'f':
        soma_atual = 15;
        break;
      default:
        soma_atual = numero[i] - '0';
    }
    decimal += soma_atual*elevado(16, n-i);
  }
  return decimal;
}

void trocaendianess(char *hexa, char*resultado){
  int n = stringlen(hexa)-2;
  int juntou = 0, j = 0;
  for(int i = n; i >= 0; i--){
    juntou++;
    if(juntou == 2){
      resultado[j++] = hexa[i];
      resultado[j++] = hexa[i+1];
      juntou = 0;
    }
    else if(i == 0 && juntou < 2){
      resultado[j++] = '0';
      resultado[j++] = hexa[i];
    }
  }

  resultado[j++] = '\n';
  resultado[j] = '\0';
}

void intparastring(unsigned long int a, char *resultado){
  char stringinvertida[32];
  int i = 0;
  while(a != 0){
    stringinvertida[i++] = (a % 10) + '0';
    a /= 10;
  }
  stringinvertida[i] = '\0';
  invertestring(stringinvertida, resultado);
  resultado[i++] = '\n';
  resultado[i] = '\0';
}

int main()
{
  char str[20];
  int n = read(0, str, 20);
  int n_semlinha = n-1;
  unsigned long int a, decimal;
  char binario[34], binariop[36];
  char hexa[32], hexa8[32], hexap[32];
  char endianesstrocado[32], endianesstrocadodec[32];
  char stringdecimal[32], stringdecimaln[32];
  char binario32[36];

  
  if(str[1] == 'x'){
    
    decimal = hexatodecimal(&str[2]);
    intparastring(decimal, stringdecimal);
    decimalbinario(decimal, binario);
    completacom0(binario, binario32);
    binhexa(binario32, hexa8);
    poeprefixo('b', binario, binariop);
    if(binario32[0] == '1'){
      poemenos(stringdecimal, stringdecimaln);
    }
    trocaendianess(hexa8, endianesstrocado);
    a = hexatodecimal(endianesstrocado);
    intparastring(a, endianesstrocadodec);
    write(1, binariop, stringlen(binariop));
    if(binario32[0] != '1')
      write(1, stringdecimal, stringlen(stringdecimal));
    else
      write(1, stringdecimaln, stringlen(stringdecimaln));
    write(1, str, n);
    write(1, endianesstrocadodec, stringlen(endianesstrocadodec));
  }
  else if (str[0] == '-'){
    char c1binario32[34], c2binario32[34], c2binario32p[36];
    int numero_lido = char_para_int(&str[1], n_semlinha-1, 10);
    decimalbinario(numero_lido, binario);
    completacom0(binario, binario32);
    complemento1(binario32, c1binario32);
    soma_um_binario(c1binario32, c2binario32);
    poeprefixo('b', c2binario32, c2binario32p);
    binhexa(c2binario32, hexa);
    poeprefixo('x', hexa, hexap);
    trocaendianess(hexa, endianesstrocado);
    a = hexatodecimal(endianesstrocado);
    intparastring(a, endianesstrocadodec);

    write(1, c2binario32p, stringlen(c2binario32p));
    write(1, str, n);
    write(1, hexap, stringlen(hexap));
    write(1, endianesstrocadodec, stringlen(endianesstrocadodec));

  }
 else{
    int numero_lido = char_para_int(str, n_semlinha, 10);
    decimalbinario(numero_lido, binario);
    binhexa(binario, hexa);
    completacom0(binario, binario32);
    binhexa(binario32, hexa8);
    trocaendianess(hexa8, endianesstrocado);
    a = hexatodecimal(endianesstrocado);
    intparastring(a, stringdecimal);
    poeprefixo('b', binario, binariop);
    poeprefixo('x', hexa, hexap);
    write(1, binariop, stringlen(binariop));
    write(1, str, n);
    write(1, hexap, stringlen(hexap));
    write(1, stringdecimal, stringlen(stringdecimal));
  }


  return 0;
}

void _start(){
  main();
}
