#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

typedef struct secao{
    unsigned char shname[4];
    unsigned char endereco[4];
    unsigned char vma[4];
    unsigned char size[4];
    int existe;
    unsigned char string_nome[20];
}secao;

typedef struct simbolo{
    unsigned char offset[4];
    unsigned char endereco[4];
    unsigned char size[4];
    unsigned char escopo[1];
    int existe;
    unsigned char nome[20];
    int global;
    unsigned char secao_associada[1];
    int secao_associada_int;
} simbolo;

typedef struct instrucao{
  unsigned char codigo[4];
  unsigned char nome[20];
  unsigned char binario[33];
  unsigned char opcode[8];
  unsigned long int valor_int;
}instrucao;

unsigned char* stringcpy(unsigned char* string_copia, const unsigned char* string_original){
    if (string_copia == NULL)
        return NULL;
    unsigned char *ponteiro_copia = string_copia;
    int n = 0;
    while (*string_original != '\0' || n == 0){
        *string_copia = *string_original;
        string_copia++;
        string_original++;
        n++;
    }
 
    *string_copia = '\0';
    return ponteiro_copia;
}

int stringcmp(const unsigned char *str1, const unsigned char *str2){
    while (*str1){
        if (*str1 != *str2) {
            break;
        }
        str1++;
        str2++;
    }
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

void troca(simbolo* item1, simbolo* item2){
    simbolo aux = *item1;
    *item1 = *item2;
    *item2 = aux;
}

int stringlen(unsigned char *string){
  unsigned char atual = string[0];
  int tamanho = 0;
  while(atual != '\0'){
    tamanho++;
    atual = string[tamanho];
  }
  return tamanho;
}

unsigned long int elevado(int base, int vezes){
  unsigned long int resultado = 1;
  for(int i = 0; i < vezes; i++){
    resultado *= base;
  }
  return resultado;
}

void invertestring(unsigned char *string, unsigned char *stringinvertida){
  int tamanho = stringlen(string);
  for(int i = 0; i < tamanho; i++){
    stringinvertida[i] = string[tamanho-i-1];
  }
}

void completacom0(unsigned char *binario, unsigned char *resultado){
  int tamanho = stringlen(binario)-2;
  int j = 0;
  for(int i = 0; i < 33; i++){
    if(32-i-1 > tamanho){
      resultado[i] = '0';
    }
    else{
      resultado[i] = binario[j++];
    }
  }
  resultado[33] = '\0';
}

int soma_quarteto_binario(unsigned char digito1, unsigned char digito2, unsigned char digito3, unsigned char digito4){
  return (digito1 - '0')*8 + (digito2 - '0')*4 + (digito3 - '0')*2 + (digito4 - '0');
}

void binhexa(unsigned char* bin, unsigned char *resultado){
  int i, j=0, juntou = 0, valor_quarteto, n = stringlen(bin)-2; 
  unsigned char digito_atual;
  unsigned char hexainvertido[34];
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
  resultado[j+1] = '\0';
}

unsigned long int valor_inteiro(unsigned char *numero, int tamanho){
    //Dado um vetor de chars em que cada char representa dois bytes, devolve o valor total em decimal desse vetor

    int multiplicador = 0;
    unsigned long int resultado2 = 0, atual;
    for(int i = 0; i < tamanho; i++){
        atual = elevado(16, multiplicador);
        resultado2 += atual*(numero[i]);
        multiplicador += 2;
    }
    return resultado2;
}

void bubbleSort(simbolo *simbolos, int n){
    int i, j;
    for(i = 0; i < n - 1; i++)
        for(j = 0; j < n - i - 1; j++)
            if(valor_inteiro(simbolos[j].endereco, 4) > valor_inteiro(simbolos[j+1].endereco, 4))
                troca(&simbolos[j], &simbolos[j+1]);
}

void decimalbinario(unsigned long int numero, unsigned char *binario){
  int i = 0,resto;
  unsigned char binarioinvertido[34];
  while(numero >= 1){
    resto = numero % 2;
    numero /= 2;
    binarioinvertido[i] = (resto+'0');
    i++;
  }
  binarioinvertido[i] = '\0';
  invertestring(binarioinvertido, binario);
  binario[i+1] = '\0';
}

void printa_tabela_secoes(secao *secoes, int n_secoes, unsigned char *nome_arquivo){
    write(1, "\n", 1);
    write(1, nome_arquivo, stringlen(nome_arquivo));
    char *s = ": file format elf32-littleriscv\n\nSections:\nIdx Name\tSize\t VMA\n";
    write(1, s, stringlen((unsigned char *)s));
    unsigned char numero[1], numero2[1];
    numero[0] = '0';
    unsigned char atual_bin[32], atual_bin32[34];
    unsigned char atual_hexa[9];
    atual_hexa[8] = '\0';
    int size_atual;
    for(int i = 0; i < n_secoes; i++){
      size_atual = valor_inteiro(secoes[i].size, 4);
      if(i == 1){
        size_atual*= 2;
      }
        
        decimalbinario(size_atual, atual_bin);
        completacom0(atual_bin, atual_bin32);
        binhexa(atual_bin32, atual_hexa);
        write(1, "  ", 2);
        if((i/10) > 0){
          numero2[0] = '0' + i/10;
          write(1, numero2, 1);
        }
        write(1, numero, 1);
        write(1, " ", 1);
        write(1, secoes[i].string_nome, stringlen(secoes[i].string_nome));
        if(i == 0)
            write(1, "\t", 1);
        write(1, "\t", 1);
        write(1, atual_hexa, 8);
        decimalbinario(valor_inteiro(secoes[i].vma, 4), atual_bin);
        completacom0(atual_bin, atual_bin32);
        binhexa(atual_bin32, atual_hexa);
        write(1, " ", 1);
        write(1, atual_hexa, 8);
        write(1, "\n", 1);
        numero[0]++;
        if(i % 10 == 0){
          numero2[0]++;
        }
    }
    write(1, "\n", 1);
} 

void printa_tabela_simbolos(secao *secoes, simbolo *simbolos, int n_simbolos, int n_secoes, unsigned char *nome_arquivo){
    write(1, "\n", 1);
    write(1, nome_arquivo, stringlen(nome_arquivo));
    char *s = ": file format elf32-littleriscv\n\nSYMBOL TABLE:\n";
    write(1, s, stringlen((unsigned char *)s));
    unsigned char atual_bin[32], atual_bin32[34];
    unsigned char atual_hexa[9];
    atual_hexa[8] = '\0';
    for(int i = 0; i < 7; i++){
      atual_hexa[i] = 0;
      atual_bin[i] = 0;
    }
    for(int i = 0; i < 32; i++){
      atual_bin32[i] = 0;
    }
    for(int i = 0; i < n_simbolos; i++){
        decimalbinario(valor_inteiro(simbolos[i].endereco, 4), atual_bin);
        completacom0(atual_bin, atual_bin32);
        binhexa(atual_bin32, atual_hexa);
        write(1, atual_hexa, 8);
        write(1, " ", 1);
        if(simbolos[i].global)
            write(1, "g\t", 2);
        else
            write(1, "l\t", 2);
        if(simbolos[i].secao_associada_int < n_secoes)
            write(1, secoes[simbolos[i].secao_associada_int].string_nome, stringlen(secoes[simbolos[i].secao_associada_int].string_nome));
        else
            write(1, "*ABS*", 5);
        write(1, "\t", 1);
        decimalbinario(valor_inteiro(simbolos[i].size, 4), atual_bin);
        completacom0(atual_bin, atual_bin32);
        binhexa(atual_bin32, atual_hexa);
        write(1, atual_hexa, 8);
        write(1, " ", 1);
        write(1, simbolos[i].nome, stringlen(simbolos[i].nome));
        write(1, "\n", 1);
    }
}

void printa_instrucoes(secao *secoes, simbolo *simbolos, instrucao *instrucoes, int n_instrucoes, int n_simbolos, unsigned char *nome_arquivo, int n_text){
    write(1, "\n", 1);
    write(1, nome_arquivo, stringlen(nome_arquivo));
    char *s = ": file format elf32-littleriscv\n\n\nDisassembly of section .text:\n";
    write(1, s, stringlen((unsigned char *)s));

    int n_simbolos_text = 0;
    simbolo simbolos_text[20];
    for(int i = 0; i < n_simbolos; i++){
      if(!stringcmp(secoes[simbolos[i].secao_associada_int].string_nome, (unsigned char *)".text") && simbolos[i].existe && secoes[simbolos[i].secao_associada_int].existe){
        stringcpy(simbolos_text[n_simbolos_text].endereco, simbolos[i].endereco);
        stringcpy(simbolos_text[n_simbolos_text].nome, simbolos[i].nome);
        n_simbolos_text++;
      }
    }
    bubbleSort(simbolos_text, n_simbolos_text);
    unsigned char atual_bin[32], atual_bin32[34], atual_hexa[9], atual_hexa2[9], atual;
    atual_hexa[8] = '\0';
    atual_hexa2[8] = '\0';
    int n = 0, instrucoes_printadas = 0;
    unsigned long size_rotulo;
    for(int i = 0; i < n_simbolos_text; i++){
      decimalbinario(valor_inteiro(simbolos_text[i].endereco, 4), atual_bin);
      completacom0(atual_bin, atual_bin32);
      binhexa(atual_bin32, atual_hexa);
      write(1, "\n", 1);
      write(1, atual_hexa, 8);
      write(1, " <", 2);
      write(1, simbolos_text[i].nome, stringlen(simbolos_text[i].nome));
      write(1, ">:\n", 3);
      if(i != n_simbolos_text-1){
        size_rotulo = valor_inteiro(simbolos_text[i+1].endereco, 4) - valor_inteiro(simbolos_text[i].endereco, 4);
      }
      else{
        size_rotulo = valor_inteiro(secoes[n_text].size, 4) - valor_inteiro(simbolos_text[i].endereco, 4);
      }
      for(int j = 0; j < size_rotulo/4; j++){
        for(int k = 0; k < 34; k++){
          if(k < 32){
            atual_bin[k] = '0';
          }
          atual_bin32[k] = '0';
        }
        n = 0;
        atual = atual_hexa[0];

        while (atual == '0'){
            n++;
            atual = atual_hexa[n];
        }
        write(1, &atual_hexa[n], 8-n);
        write(1, ": ", 2);

        decimalbinario(valor_inteiro(instrucoes[instrucoes_printadas].codigo, 4), atual_bin);
        completacom0(atual_bin, atual_bin32);
        binhexa(atual_bin32, atual_hexa2);
        
        write(1, &atual_hexa2[6], 2);
        write(1, " ", 1);
        write(1, &atual_hexa2[4], 2);
        write(1, " ", 1);
        write(1, &atual_hexa2[2], 2);
        write(1, " ", 1);
        write(1, &atual_hexa2[0], 2); 
        write(1, " ", 1);
        write(1, "\n", 1);
        for(int k = 0; k < 34; k++){
            if(k < 32){
              atual_bin[k] = '0';
            }
            atual_bin32[k] = '0';
          }
        decimalbinario(valor_inteiro(simbolos_text[i].endereco, 4) + 4*(j+1), atual_bin);
        completacom0(atual_bin, atual_bin32);
        binhexa(atual_bin32, atual_hexa);
        instrucoes_printadas++;

        if(instrucoes_printadas == n_instrucoes)
          break;

          }
    }
}

int main(int argc, char *argv[]){
    //if(argc < 3)
        //return 1;
    
    int bytes_lidos = 0;
    int arquivo = open(argv[2], O_RDONLY);
    unsigned char inutil[1];
    unsigned char e_shoff[4];
    unsigned char e_shnum[2];
    unsigned char e_shstrndx[2];
    for(int i = 0; i < 32; i++)
        read(arquivo, inutil, 1);
    read(arquivo, e_shoff, 4);
    for(int i = 0; i < 12; i++)
        read(arquivo, inutil, 1);
    read(arquivo, e_shnum, 2);
    read(arquivo, e_shstrndx, 2);

    bytes_lidos = 52;

    int offset_secoes = valor_inteiro(e_shoff, 4);
    int n_secoes = valor_inteiro(e_shnum, 2);
    int n_shstrndx = valor_inteiro(e_shstrndx, 2);
    for(int i = 0; i < offset_secoes-bytes_lidos; i++)
        read(arquivo, inutil, 1);

    bytes_lidos = offset_secoes;

    secao secoes[20];
    for(int i = 0; i < 40; i++){
        if(i < n_secoes){
            secoes[i].existe = 1;
            for(int j = 0; j < 20; j++){
                secoes[i].string_nome[j] = 0;
                if(j < 4)
                  secoes[i].size[j] = 0;
            }
        }
        else
            secoes[i].existe = 0;
    }
    int tamanho_shstrtab;
    unsigned char endereco_shstrtab[4];
    for(int i = 0; i < n_secoes; i++){

        read(arquivo, secoes[i].shname, 4);
        for(int j = 0; j < 0x8; j++)
            read(arquivo, inutil, 1);
        read(arquivo, secoes[i].vma, 4);
        read(arquivo, secoes[i].endereco, 4);
        read(arquivo, secoes[i].size, 4);
        if(i == n_shstrndx){
            for(int j = 0; j < 4; j++)
                endereco_shstrtab[j] = secoes[i].endereco[j];
            tamanho_shstrtab = valor_inteiro(secoes[i].size, 4);
        }
        for(int j = 0; j< 0x10; j++)
            read(arquivo, inutil, 1);
    }
        
    int offset_shstrtab = valor_inteiro(endereco_shstrtab, 4);
    

    close(arquivo);
    arquivo = open(argv[2], O_RDONLY);
    for(int i = 0; i < offset_shstrtab; i++)
        read(arquivo, inutil, 1);
    bytes_lidos = offset_shstrtab;
    int bytes_lidos_nasecao = 0, n_atual;
    for(int i = 0; i < n_secoes; i++){
        if(i != n_secoes-1){
            n_atual = valor_inteiro(secoes[i+1].shname, 4) - valor_inteiro(secoes[i].shname, 4);
            read(arquivo, secoes[i].string_nome, n_atual);
            secoes[i].string_nome[n_atual] = '\0';
            bytes_lidos_nasecao += n_atual;
        }
        else{
            read(arquivo, secoes[i].string_nome, tamanho_shstrtab - bytes_lidos_nasecao);
        }
    }
    if(argv[1][1] == 'h'){
        printa_tabela_secoes(secoes, n_secoes, (unsigned char *)argv[2]);
        return 0;
    }
    

    int endereco_symtab, endereco_strtab, tamanho_symtab, tamanho_strtab;
    for(int i = 0; i < n_secoes; i++){
        if(!stringcmp(secoes[i].string_nome, (unsigned char *)".symtab")){
            endereco_symtab = valor_inteiro(secoes[i].endereco, 4);
            tamanho_symtab = valor_inteiro(secoes[i].size, 4);
        }
        else if(!stringcmp(secoes[i].string_nome, (unsigned char *)".strtab")){
            endereco_strtab = valor_inteiro(secoes[i].endereco, 4);
            tamanho_strtab = valor_inteiro(secoes[i].size, 4);
        }
    }
    close(arquivo);
    arquivo = open(argv[2], O_RDONLY);
    simbolo simbolos[20];
    int n_simbolos = (tamanho_symtab - 16)/16;
    for(int i = 0; i < n_simbolos; i++){
        simbolos[i].existe = 1;
    }
    for(int i = 0; i < endereco_symtab + 0x10; i++)
        read(arquivo, inutil, 1);
    for(int i = 0; i < n_simbolos; i++){
        read(arquivo, simbolos[i].offset, 4);
        read(arquivo, simbolos[i].endereco, 4);
        read(arquivo, simbolos[i].size, 4);
        read(arquivo, simbolos[i].escopo, 1);
        read(arquivo, inutil, 1);
        read(arquivo, simbolos[i].secao_associada, 1);
        simbolos[i].secao_associada_int = valor_inteiro(simbolos[i].secao_associada, 1);
        read(arquivo, inutil, 1);
        if(simbolos[i].escopo[0] == 0)
            simbolos[i].global = 0;
        else if (simbolos[i].escopo[0] == 16)
            simbolos[i].global = 1;
    }
    close(arquivo);
    arquivo = open(argv[2], O_RDONLY);
    for(int i = 0; i < endereco_strtab + valor_inteiro(simbolos[0].offset, 4); i++)
        read(arquivo, inutil, 1);
    
    bytes_lidos_nasecao = 0;
    for(int i = 0; i < n_simbolos; i++){
        for(int j = 0; j < 20; j++){
            simbolos[i].nome[j] = 0;
        }
    }
    for(int i = 0; i < n_simbolos; i++){
        if(i != n_simbolos - 1){
            n_atual = valor_inteiro(simbolos[i+1].offset, 4) - valor_inteiro(simbolos[i].offset, 4);
            read(arquivo, simbolos[i].nome, n_atual);
            bytes_lidos_nasecao += n_atual;
        }
        else{
            read(arquivo, simbolos[i].nome, tamanho_strtab - bytes_lidos_nasecao);
        }
    }

    if(argv[1][1] == 't'){
        printa_tabela_simbolos(secoes, simbolos, n_simbolos, n_secoes, (unsigned char *)argv[2]);
        return 0;
    }

    int endereco_text, tamanho_text, n_text;
    for(int i = 0; i < n_secoes; i++){
      if(!stringcmp(secoes[i].string_nome, (unsigned char *)".text")){
        endereco_text = valor_inteiro(secoes[i].endereco, 4);
        tamanho_text = valor_inteiro(secoes[i].size, 4);
        n_text = i;
      }
    }

    close(arquivo);
    arquivo = open(argv[2], O_RDONLY);

    for(int i = 0; i < endereco_text; i++)
      read(arquivo, inutil, 1);
    int n_instrucoes = tamanho_text/4;
    instrucao instrucoes[40];
    unsigned char binario_atual[33];
    for(int i = 0; i < 33; i++){
      binario_atual[i] = '\0';
    }

    for(int i = 0; i < n_instrucoes; i++){
      read(arquivo, instrucoes[i].codigo, 4);
      instrucoes[i].valor_int = valor_inteiro(instrucoes[i].codigo, 4);
      decimalbinario(instrucoes[i].valor_int, binario_atual);
      completacom0(binario_atual, instrucoes[i].binario);
      for(int j = 0; j < 7; j++){
        instrucoes[i].opcode[j] = instrucoes[i].binario[26+j];
      }
      instrucoes[i].opcode[7] = 0;
      if(!stringcmp(instrucoes[i].opcode, (unsigned char *)"0110111"))
        stringcpy(instrucoes[i].nome, (unsigned char *)"lui");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0010111"))
        stringcpy(instrucoes[i].nome, (unsigned char *)"auipc");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"1101111"))
        stringcpy(instrucoes[i].nome, (unsigned char *)"jal");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"1100111"))
        stringcpy(instrucoes[i].nome, (unsigned char *)"jalr");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"1100011") && !instrucoes[i].binario[12] && !instrucoes[i].binario[13] && !instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"beq");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"1100011") && instrucoes[i].binario[12] && !instrucoes[i].binario[13] && !instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"bne");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"1100011") && !instrucoes[i].binario[12] && !instrucoes[i].binario[13] && instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"blt");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"1100011") && instrucoes[i].binario[12] && !instrucoes[i].binario[13] && instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"bge");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"1100011") && !instrucoes[i].binario[12] && instrucoes[i].binario[13] && instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"bltu");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"1100011") && instrucoes[i].binario[12] && instrucoes[i].binario[13] && instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"bgeu");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0000011") && !instrucoes[i].binario[12] && !instrucoes[i].binario[13] && !instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"lb");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0000011") && instrucoes[i].binario[12] && !instrucoes[i].binario[13] && !instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"lh");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0000011") && !instrucoes[i].binario[12] && instrucoes[i].binario[13] && !instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"lw");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0000011") && !instrucoes[i].binario[12] && !instrucoes[i].binario[13] && instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"lbu");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0000011") && instrucoes[i].binario[12] && !instrucoes[i].binario[13] && instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"lhu");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0100011") && !instrucoes[i].binario[12] && !instrucoes[i].binario[13] && !instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"sb");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0100011") && instrucoes[i].binario[12] && !instrucoes[i].binario[13] && !instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"sh");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0100011") && !instrucoes[i].binario[12] && instrucoes[i].binario[13] && !instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"sw");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0010011") && !instrucoes[i].binario[12] && !instrucoes[i].binario[13] && !instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"addi");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0010011") && !instrucoes[i].binario[12] && instrucoes[i].binario[13] && !instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"slti");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0010011") && instrucoes[i].binario[12] && instrucoes[i].binario[13] && !instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"sltiu");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0010011") && !instrucoes[i].binario[12] && !instrucoes[i].binario[13] && instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"xori");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0010011") && !instrucoes[i].binario[12] && instrucoes[i].binario[13] && instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"ori");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0010011") && instrucoes[i].binario[12] && instrucoes[i].binario[13] && instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"andi");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0010011") && instrucoes[i].binario[12] && !instrucoes[i].binario[13] && !instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"slli");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0010011") && instrucoes[i].binario[12] && !instrucoes[i].binario[13] && instrucoes[i].binario[14] && !instrucoes[i].binario[30])
        stringcpy(instrucoes[i].nome, (unsigned char *)"srli");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0010011") && instrucoes[i].binario[12] && !instrucoes[i].binario[13] && instrucoes[i].binario[14] && instrucoes[i].binario[30])
        stringcpy(instrucoes[i].nome, (unsigned char *)"srai");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0110011") && !instrucoes[i].binario[12] && !instrucoes[i].binario[13] && !instrucoes[i].binario[14] && !instrucoes[i].binario[30])
        stringcpy(instrucoes[i].nome, (unsigned char *)"add");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0110011") && !instrucoes[i].binario[12] && !instrucoes[i].binario[13] && !instrucoes[i].binario[14] && instrucoes[i].binario[30])
        stringcpy(instrucoes[i].nome, (unsigned char *)"sub");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0110011") && instrucoes[i].binario[12] && !instrucoes[i].binario[13] && !instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"sll");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0110011") && !instrucoes[i].binario[12] && instrucoes[i].binario[13] && !instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"slt");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0110011") && instrucoes[i].binario[12] && instrucoes[i].binario[13] && !instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"sltu");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0110011") && !instrucoes[i].binario[12] && !instrucoes[i].binario[13] && instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"xor");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0110011") && instrucoes[i].binario[12] && !instrucoes[i].binario[13] && instrucoes[i].binario[14] && !instrucoes[i].binario[30])
        stringcpy(instrucoes[i].nome, (unsigned char *)"srl");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0110011") && instrucoes[i].binario[12] && !instrucoes[i].binario[13] && instrucoes[i].binario[14] && instrucoes[i].binario[30])
        stringcpy(instrucoes[i].nome, (unsigned char *)"sra");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0110011") && !instrucoes[i].binario[12] && instrucoes[i].binario[13] && instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"or");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0110011") && instrucoes[i].binario[12] && instrucoes[i].binario[13] && instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"and");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0001111") && !instrucoes[i].binario[12] && !instrucoes[i].binario[13] && !instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"fence");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"0001111") && instrucoes[i].binario[12] && !instrucoes[i].binario[13] && !instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"fence.i");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"1110011") && !instrucoes[i].binario[12] && !instrucoes[i].binario[13] && !instrucoes[i].binario[14] && !instrucoes[i].binario[20])
        stringcpy(instrucoes[i].nome, (unsigned char *)"ecall");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"1110011") && !instrucoes[i].binario[12] && !instrucoes[i].binario[13] && !instrucoes[i].binario[14] && instrucoes[i].binario[20])
        stringcpy(instrucoes[i].nome, (unsigned char *)"ebreak");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"1110011") && instrucoes[i].binario[12] && !instrucoes[i].binario[13] && !instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"csrrw");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"1110011") && !instrucoes[i].binario[12] && instrucoes[i].binario[13] && !instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"csrrs");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"1110011") && instrucoes[i].binario[12] && instrucoes[i].binario[13] && !instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"csrrc");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"1110011") && instrucoes[i].binario[12] && !instrucoes[i].binario[13] && instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"csrrwi");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"1110011") && !instrucoes[i].binario[12] && instrucoes[i].binario[13] && instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"csrrsi");
      else if (!stringcmp(instrucoes[i].opcode, (unsigned char *)"1110011") && instrucoes[i].binario[12] && instrucoes[i].binario[13] && instrucoes[i].binario[14])
        stringcpy(instrucoes[i].nome, (unsigned char *)"csrrci");
      else
        stringcpy(instrucoes[i].nome, (unsigned char *)"<unknown>");


    } 

    if(argv[1][1] == 'd'){
      printa_instrucoes(secoes, simbolos, instrucoes, n_instrucoes, n_simbolos, (unsigned char *)argv[2], n_text);
    }

    return 0;
}
