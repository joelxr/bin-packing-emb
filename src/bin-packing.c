/**
 *  \file bin-packing.c
 *  \brief
 *  
 *  Esse arquivo contem a implementação do famoso algoritmo de empacotamento de números,
 *  conhecido como "Bin-Packing".
 *  Esse programa opera sob uma coleção de números que podem ser informados como parâmetro
 *  ou gerados de forma pseudo-aleatoria. Esses numéros são armazenados em pacotes que
 *  chamam-se BINs. Um BIN nada mais é do que uma coleção de números onde a soma deles
 *  não ultrapassa um valor pré-determinado.
 *
 *  Considerar que:
 *    
 *    1. Foi implementado a versão de uma dimensão que consiste em agrupar os BINs em forma de
 *       um array de uma única dimensão, as demais implementações utilizam arrays de maiores
 *       dimensões.
 *    2. A estratégia de agrupamento utilizada foi a "First Fit Decresing", que consiste em
 *       ler a entrada de némueros em ordem decrescente e empacota-los no primeiro BINs que
 *       possuir espaço.
 * 
 * Parâmetros do programa:
 *
 *    - [arg 1] : Quantidade de itens que serão empacotados.
 *    - [arg 2] : Tamanho do BIN, ou seja valor máximo que poderá atingir a soma dos itens
 *                armazenados em um BIN.
 *    - [arg 3] : Valor míximo para os números.
 *    - [arg 4] : Valor mánimo para os números.
 *    - [arg 5] : Primeiro item da lista de números. A partir do quinto parâmetro, o usuário
 *                poderá passar os números que quiser para que sejam empactados.
 *    - [arg 6] : Segundo item da lista de números.
 *    - [arg 7] : Terceiro item da lista de números.
 *    - [arg N] : Enésimo item da lista de números.
 * 
 * Exemplos de uso:
 *    - <tt>./bin-packing.o 2000 100 20 100</tt>
 *    - <tt>./bin-packing.o 2000 100 20 100 1 2 3 4 5 6 7 8 9 10 12 13 14 15 17</tt>
 *
 *  \author Rocha, Joel (joelxr@gmail.com)
 *  \author Santos, Luciente dos (lucienesantosmc@gmail.com)
 *  \date 2013-11-13
 *  \copyright GPLv2
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/timeb.h>

/** 
 * Estrutra que representa um BIN
 */
typedef struct bin 
{
   unsigned short int *itens; /** Ponteiro para os itens que o BIN possui */
   unsigned short int left; /** Representa o espaço dísponivel no BIN */
   unsigned short int count; /** Qauntidade de itens existentes no BIN */
} bin;

/**
 * Estrutura que reprsenta uma lista de BINs
 */
typedef struct bin_list 
{
   bin *itens; /** Ponteiro para os BINs que a lista possui */
   unsigned short int count; /** Representa a quantidade de BINs existentes na lista */
} bin_list;

/** A Quantidade de números que devem ser colocados nos BINs */
unsigned short int NUMBERS_QUANTITY;
/** O tamanho do BIN, ou seja, o valor total da soma dos numeros que nele */
unsigned short int BIN_SIZE;
/** O valor minimo que deve ser gerado os numeros */
unsigned short int NUMBERS_MINIMUM;
/** O valor máximo que deve ser gerado os numeros */
unsigned short int NUMBERS_MAXIMUM; 

int comparison_numbers (const void * a, const void * b);
int create_numbers_array (unsigned short int *values);
bin* create_empty_bin ();
bin_list* create_empty_bin_list ();
int fill_bins (unsigned short int *values, bin_list *bins);
int free_bins (bin_list *bins);
int generate_random_number (unsigned short int min, unsigned short int max);
int insert_number_bin (bin *b, unsigned short int num);
int insert_bin_list (bin_list *list, bin *b);
int print_bin(bin *b);
int print_list_bins (bin_list *bins);
int print_numbers (unsigned short int *values);
int sort_numbers_array (unsigned short int *values);

/**
 * Função principal do programa, responsável por executar funções 
 * que definem o comportamento do algoritmo.
 * 
 * \param argc Quantidade de argumentos passados ao programa.
 * \param argv Vetor que contem os argumentos passados para o programa.
 * \return Retorna zero caso tenha sucesso na execução.
 * \see create_empty_bin_list 
 * \see sort_numbers_array
 * \see print_numbers
 * \see fill_bins
 * \see print_list_bins
 * \see free_bins
 * \see NUMBERS_QUANTITY
 * \see NUMBERS_MINIMUM
 * \see NUMBERS_MAXIMUM
 * \see BIN_SIZE
 */ 
int main(int argc, char **argv)
{
   unsigned short int i;
   unsigned short int *values; /** Usa-se ponteiro para armazenar a lista de números. */
   bin_list *bins; /** Usa-se ponteiro contem a lsita de BINs gerados dinamicamente. */

   /**
    * Verifica-se os argumentos passados, ou seja, se foi passado a quantidade necessária. 
    * Se não, informa o que é preciso para execução e encerra o programa.
    */
   if (argc < 5)
   {
      printf("Passar os argumentos do programa.\n");
      printf("1 - Quantidade de números para empacotar \n");
      printf("2 - Tamanhos dos BINs \n");
      printf("3 - Valor mínimo dos números \n");
      printf("4 - Valor máximo dos números \n");
      printf("5 - Valores a serem empacotados (Opcional) \n");
      exit(1);
   }

   /** 
    * Atribui os argumentos as variavéis globais do programa, para
    * que seja utilizado no restante do rpograma.
    */
   NUMBERS_QUANTITY = atoi(argv[1]);
   BIN_SIZE = atoi(argv[2]);
   NUMBERS_MINIMUM = atoi(argv[3]);
   NUMBERS_MAXIMUM = atoi(argv[4]);

   /**
    * Caso tenha sido passado mais de cinco argumentos para o programa,
    * significa que a lista de números foi informada pelo usuário e,
    * portanto, não será gerada aleatoriamente.
    */
   if (argc > 5)
   {
      values = malloc(sizeof(int)*argc-5);
      NUMBERS_QUANTITY = argc -5;

      for (i = 0; i < argc-5; i++)
         values[i] = atoi( argv[i+5]);
   }
   else
   {
      values = malloc(sizeof(int)*NUMBERS_QUANTITY);
      create_numbers_array (values);
   }

   /**
    * \attention
    * Caso não tenha sido possível alocar memória para os números gerados ou
    * informados o programa é encerrado devido a indisponibilidade de memória.
    */
   if (values == NULL)
      exit(1);

   /** Inicialisa a lista de BINs.*/
   bins = create_empty_bin_list();
   /** Ordena de forma descrescente os números para empacotar. */
   sort_numbers_array (values);
   /** Imprime os números gerados e devidamente ordenados. */
   print_numbers(values);
   /** Preenche os BINS, ou seja, ler a lista de números e gera os BINs necessários. */ 
   fill_bins (values, bins);
   /** Imprime os BINs que foram gerados. */
   print_list_bins (bins);
   /** Por fim, libera todos os recursos que foram utilizados. */
   free_bins (bins);

   free (values);
   return 0;
}

/**
 * Função necessária para limpeza de dados utilizados durante a execução do programa.
 * Todos os mallocs, reallocs são liberados aqui evitando qualquer tipo de
 * memory leak.
 *
 * \param bins Ponteiro para a lista de BINs gerados pelo programa.
 * \return Zero aopós finalizado.
 * \see free
 */ 
int free_bins (bin_list *bins)
{
   unsigned short int i;

   /** 
    * Percorre a lista de BINs passada como parametro, para cada BIN existente
    * é liberado o array de números que existe para armazenar os itens.
    */
   for (i = 0; i < bins->count; i++)
   {
      bin *b = (bins->itens + i);
      free(b->itens);
   }

   /** Por fim, libera a lista de BINs utilizada pelo programa. */
   free(bins->itens);
   free(bins);

   return 0;
}

/**
 * Método que preenche os BINs com os numeros existentes no array "values" que é
 * passado como parametro.
 * Esse método é responsável por criar, se necessário, novos bins e colocar na lista de BINS.
 *
 * \param values Ponteiro para o array que armazena os números que devem ser empacotados nos BINs.
 * \param bins Ponteiro para a lista contendo os BINs do programa.
 * \return  0 - Quando a lista de BINs foi gerada com sucesso, 
 *          1 - Quando em algum momento não foi possivel colcoar um novo BIN na lista de BINs.
 * \see insert_number_bin
 * \see insert_bin_list
 * \see NUMBERS_QUANTITY
 */
int fill_bins (unsigned short int *values, bin_list *bins)
{
   unsigned short int i;
   unsigned short int j;
   char hasInserted = 1; /* 0 - conseguiu inserir, 1 - não inseriu ainda */ 

   /** Percorre todos os números contidos em \em values, a fim de adionar o número em um respectivo BIN. */
   for (i = 0; i < NUMBERS_QUANTITY; i++)
   {
      unsigned short int num = values[i];

      /** Para cada número, percorre por todos os BINs existentes na lista de BINs, e tenta inserir o número em algum. */
      for (j = 0; j < bins->count; j++)
      {
         bin* aux = (bins->itens + j);

         /** 
          * É utilizado uma flag de controle para verificar se foi possivel inserir o número, os BINs são verificados
          * de um a um. Caso tenha conseguido inserir, pega-se o próximo número e aplica o mesmo raciocinio..
          */ 
         hasInserted = insert_number_bin(aux, num);

         /* Conseguiu inserir, que bom! :^) */
         if (hasInserted == 0)
            break;
      }

      /**
       * Quando todos os BINs são verificados e a inserção do número não foi possível, faz-se o seguinte:
       *  - Cria-se um novo BIN,
       *  - Coloca esse número nele e 
       *  - Adicona o BIN criado na lista de BINs, aqui pode ocorrer de não ser possível realizar a inserção.
       *  Daí é retornado o código "1" como forma de indicar a falha.
       */
      if (hasInserted == 1)
      {
         bin *b = create_empty_bin();
         insert_number_bin(b, num);
         hasInserted = insert_bin_list(bins, b);
      }
   }

   return hasInserted;
}

/**
 * Método usado para mostrar os BINs que foram criados na lista de BINs.
 * Percorre a lista de BINs e para cada BIN exibe os números que existem nele,
 * Assim como o espaço restante "left" e outras informações.
 *
 * \param bins Lista de BINs para imprimir.
 * \see print_bin
 */
int print_list_bins (bin_list *bins)
{
   unsigned short int i;

   /**
    * Percorre os elementos que existem na lista de BINs e para cada um deles
    * Chama a função que imprime o BIN.
    */
   for (i = 0; i < bins->count; i++)
   {
      bin *aux = (bins->itens + i);
      printf(" {%04d} ", i);
      print_bin(aux);
   }

   printf("\n\n");

   return 0;
}

/**
 * Função que recebe um intervalo de inteiros e gera um numero
 * pseudo-aleatorio dentro desse intervalo.
 *
 * \param min Valor minimo do intervalo que o número será gerado.
 * \param max Valor máximo.
 * \return O número gerado dentro do intervalo passado.
 */ 
int generate_random_number (unsigned short int min, unsigned short int max) {
   unsigned short int k;
   double d;
   d = (double) rand() / ((double) RAND_MAX + 1);
   k = d * (max - min + 1);
   return min + k;
}

/** 
 * Método usado para gerar a lista de inteiros que serão utilizados para 
 * serem colocados nos respectivos BINs. O números são gerados de forma
 * aleatória. IMPORTANTE: A média global entre eles deve ser igual ou 
 * superior a 20.
 *
 * \param values Ponteiro para o array onde foram inseridos os números para
 *    serem empacotados
 * \see generate_random_number
 * \see NUMBERS_QUANTITY
 * \see NUMBERS_MINIMUM
 * \see NUMBERS_MAXIMUM
 */
int create_numbers_array (unsigned short int *values)
{
   unsigned short int i;

   for (i = 0; i < NUMBERS_QUANTITY; i++)
   {
      /** Os elementos da lista devem ter valores entre a media desejada e o espaço disponivel nos BINs */
      values[i] = generate_random_number(NUMBERS_MINIMUM , NUMBERS_MAXIMUM);
   }

   return 0;
}

/** 
 * Método utlizado para ordernar o array de números, a ordenação é decrescente e é preciso ordenar para 
 * utilizar a menor quatidade possíveis de BINs.
 *
 * \param values Ponteiro para o array onde estão os números para serem empacotados
 * \see qsort
 * \see NUMBERS_QUANTITY
 * \see comparison_numbers
 */
int sort_numbers_array (unsigned short int *values)
{
   /** Utiliza o Quick Sort para ordernar os numeros. */
   qsort(values, NUMBERS_QUANTITY, sizeof(int), comparison_numbers);
   return 0;
}

/**
 * Função utilizada para comparar dois numeros, usa-se essa função de comparação
 * para estabelecer o critério de ordenação do array de numeros.
 * No caso, essa função é responsavel por estabeler a ordem decrescente do elementos do array.
 *
 * \param a Ponteiro para o primeiro número 
 * \param b Ponteiro para o segundo número
 * \return Diferença entre os números
 */ 
int comparison_numbers (const void * a, const void * b) {
   return ( *(int*)b - *(int*)a );
}

/**
 * Função responsável por criar um BIN vazio para que sejam inseridos os números nele..
 * Obs.: Esse método pode encerrar o programa caso não exista memória para alocar o novo BIN.
 *
 * \return Um ponteiro do tipo "bin" que aponta para o BIN criado.
 * \see BIN_SIZE
 */ 
bin* create_empty_bin ()
{
   bin *b = malloc(sizeof(bin));
   /*int *itens = malloc(sizeof(int));
   
   if (itens == NULL)
      exit(1);*/

   if (b == NULL)
      exit(1);

   /*b->itens = itens;*/
   b->left = BIN_SIZE;
   b->count = 0;
   return b;
}

/**
 * Função responsável por criar uma lista de BINs vazia.
 *
 * \return Um ponteiro para o tipo "bin_list" que aponta para a lista criada.
 */ 
bin_list* create_empty_bin_list ()
{
   bin_list *list = malloc(sizeof(bin_list));

   if (list == NULL)
      exit(0);

   list->count = 0;
   return list;
}

/**
 * Função responsável por inserir um dado número num BIN passado como parametro, 
 * Na inserção de um número em um BIN é levado em consideração o espaço
 * disponivel nele, ou seja, a variavel "left".
 *
 * \param b Ponteiro para o BIN onde será inserido o numero.
 * \param num Número que deve ser inserido no BIN.
 * \return 1 caso tenha conseguido inserir, ou 0 caso contrário.
 */ 
int insert_number_bin (bin *b, unsigned short int num) {

   if (num <= b->left) {

      if (b->count == 0)
      {
         unsigned short int *itens = malloc(sizeof(int));

         if (itens == NULL)
            exit(1);

         b->itens = itens;
      }
      else
      {
         b->itens = realloc(b->itens, sizeof(unsigned short int)*(b->count+1));
      }

      b->left -= num;
      b->itens[b->count] = num;
      b->count++;

      return 0;
   } else {
      return 1;
   }
}

/**
 * Função responsável por inserir um BIN na lista de BINS passada como
 * parametro na função, 
 *
 * \param list Lista de BINs, onde deve ser inserido o BIN.
 * \param b BIN para ser inserido na lista.
 * \return 1 caso tenha conseguido inserir o BIN na lista, ou 0 caso contrario.
 */ 
int insert_bin_list (bin_list *list, bin *b) {
   if (list->count > NUMBERS_QUANTITY) {
      return 1;
   } else {

      if (list->count == 0)
      {
         bin *bins = malloc(sizeof(bin));

         if (bins == NULL)
            exit(1);

         list->itens = bins;
      }
      else
      {
         list->itens = realloc(list->itens, sizeof(bin)*(list->count+1));
      }

      list->itens[list->count] = *b;
      list->count++;

      free(b);
      return 0;
   }
}

/**
 * Função para imprimir um BIN, mostrando as informações principais
 * dele.
 *    - Quantidade que resta para o BIN ser preenchido (left);
 *    - Qauntidade de itens inseridos no BIN, geralemente terá o memso
 *       valor da variavel de tamanho (count);
 *    - E os numeros que exitem nesse BIN (itens).
 *
 * \param bin Um BIN para ser impresso.
 */ 
int print_bin (bin *b) {
   unsigned short int *item = b->itens;
   unsigned short int i;
   printf("Left: %4d", b->left);
   printf(" | Count: %4d", b->count);
   printf(" | Itens: ");

   for (i = 0; i < b->count; i++) {
      if (i == b->count - 1)
         printf("%4d", *(item+i));
      else
         printf("%4d, ", *(item+i));
   }

   printf("\n");

   return 0;
}

/**
 * Função para imprimir os numeros que serão empacotados nos BINS.
 *
 * \param values Um ponteiro para um array de numéros..
 */ 
int print_numbers (unsigned short int *values) {
   unsigned short int i;
   unsigned short int total = 0;

   printf("\nNumbers:\n\n");

   for (i = 0; i < NUMBERS_QUANTITY; i++) {
      printf(" %4d ", values[i]);
      total += values[i];
   }

   printf("\n\nTotal: %4d\nAverage:%4d\n\n", i, total / NUMBERS_QUANTITY );

   return 0;
}

