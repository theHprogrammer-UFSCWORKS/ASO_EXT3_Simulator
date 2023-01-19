// Autor: Helder Henrique da Silva
// Data: de 29/08/2022 a 31/12/2022
// Descrição: Simulações de um sistema de arquivos EXT3 do Linux.
//
// Copyright (C) 2022 Helder Henrique da Silva. Todos os direitos reservados.

#include "auxFunction.hpp"

/**
 * @brief Inicializa um sistema de arquivos que simula EXT3
 * @param fsFileName nome do arquivo que contém sistema de arquivos que simula EXT3 (caminho do arquivo no sistema de arquivos local)
 * @param blockSize tamanho em bytes do bloco
 * @param numBlocks quantidade de blocos
 * @param numInodes quantidade de inodes
 */
void initFs(string fsFileName, int blockSize, int numBlocks, int numInodes)
{
	// Arquivo a ser aberto no modo wb+ (escrita e leitura)
	FILE *arquivo = fopen(fsFileName.c_str(), "wb+");
	if (arquivo == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}

	inicializar(arquivo, blockSize, numBlocks, numInodes);

	// Fechando o arquivo
	fclose(arquivo);
}

/**
 * @brief Adiciona um novo arquivo dentro do sistema de arquivos que simula EXT3. O sistema já deve ter sido inicializado.
 * @param fsFileName arquivo que contém um sistema de arquivos que simula EXT3.
 * @param filePath caminho completo novo arquivo dentro sistema de arquivos que simula EXT3.
 * @param fileContent conteúdo do novo arquivo
 */
void addFile(string fsFileName, string filePath, string fileContent)
{
	// Arquivo a ser aberto no modo r+
	FILE *arquivo = fopen(fsFileName.c_str(), "r+");
	if (arquivo == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}

	adicionarArquivo(arquivo, filePath, fileContent);

	fclose(arquivo);
}

/**
 * @brief Adiciona um novo diretório dentro do sistema de arquivos que simula EXT3. O sistema já deve ter sido inicializado.
 * @param fsFileName arquivo que contém um sistema sistema de arquivos que simula EXT3.
 * @param dirPath caminho completo novo diretório dentro sistema de arquivos que simula EXT3.
 */
void addDir(string fsFileName, string dirPath)
{
	// Arquivo a ser aberto no modo r+
	FILE *arquivo = fopen(fsFileName.c_str(), "r+");
	if (arquivo == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}

	adicionarDiretorio(arquivo, dirPath);

	fclose(arquivo);
}

/**
 * @brief Remove um arquivo ou diretório (recursivamente) de um sistema de arquivos que simula EXT3. O sistema já deve ter sido inicializado.
 * @param fsFileName arquivo que contém um sistema sistema de arquivos que simula EXT3.
 * @param path caminho completo do arquivo ou diretório a ser removido.
 */
void remove(string fsFileName, string path)
{
	// Arquivo a ser aberto no modo r+
	FILE *arquivo = fopen(fsFileName.c_str(), "r+");
	if (arquivo == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}

	if (path.find(".") != string::npos)
	{
		// remover arquivo

		// blockSize: tamanho do bloco
		// numBlocks: quantidade de blocos
		// numInodes: quantidade de inodes
		// root: inode do diretório raiz
		unsigned char blockSize, numBlocks, numInodes, root;

		// Posicionamento do ponteiro no inicio do arquivo e leitura dos 3 primeiros bytes.
		fseek(arquivo, 0, SEEK_SET);
		fread(&blockSize, sizeof(unsigned char), 1, arquivo);
		fread(&numBlocks, sizeof(unsigned char), 1, arquivo);
		fread(&numInodes, sizeof(unsigned char), 1, arquivo);

		// Quantidade de bytes do mapa de bits.
		unsigned char bitMapSize = getBitMapSize(numBlocks);

		// Espaço para tratamento do mapa de bits
		vector<unsigned char> bitMap(bitMapSize);

		// Espaço para tratamento dos inodes.
		vector<INODE> inodes(numInodes);

		// Espaço para o vetor de blocos.
		vector<vector<unsigned char>> blocos(numBlocks, vector<unsigned char>(blockSize));

		// Leitura dos bytes do mapa de bits, inodes, root e blocos.
		fread(&bitMap[0], sizeof(unsigned char), bitMapSize, arquivo);
		fread(&inodes[0], sizeof(INODE), numInodes, arquivo);
		fread(&root, 1, 1, arquivo);
		for (int i = 0; i < numBlocks; i++)
		{
			fread(&blocos[i][0], sizeof(unsigned char), blockSize, arquivo);
		}
		// Vetor de blocos usados - False = livre, True = usado.
		vector<bool> blocosUsados = mappingUsedBlocks(inodes, numBlocks, numInodes);

		// Nome do arquivo a ser removido.
		string arquivoNome_remove = getName(path);

		// Índice do inode do arquivo a ser removido.
		int inode_remove = getInodeIndex(arquivoNome_remove, inodes, numInodes);

		// Lista de blocos a serem liberados.
		vector<unsigned char> blocosLiberar;
		for (int i = 0; i < 3; i++)
		{
			if (inodes[inode_remove].DIRECT_BLOCKS[i] != 0x00)
			{
				blocosLiberar.push_back(inodes[inode_remove].DIRECT_BLOCKS[i]);
			}
			for (int j = 0; j < 3; j++)
			{
				if (inodes[inode_remove].INDIRECT_BLOCKS[i] != 0x00)
				{
					blocosLiberar.push_back(inodes[inode_remove].INDIRECT_BLOCKS[i]);
					for (int k = 0; k < 3; k++)
					{
						if (inodes[inode_remove].DOUBLE_INDIRECT_BLOCKS[i] != 0x00)
						{
							blocosLiberar.push_back(inodes[inode_remove].DOUBLE_INDIRECT_BLOCKS[i]);
						}
					}
				}
			}
		}

		// Atribuir 0x00 para os campos do inode do arquivo a ser removido.
		inodes[inode_remove].IS_USED = 0x00;
		inodes[inode_remove].IS_DIR = 0x00;
		inodes[inode_remove].SIZE = 0x00;
		for (int i = 0; i < 10; i++)
		{
			inodes[inode_remove].NAME[i] = 0x00;
		}
		for (int i = 0; i < 3; i++)
		{
			inodes[inode_remove].DIRECT_BLOCKS[i] = 0x00;
			inodes[inode_remove].INDIRECT_BLOCKS[i] = 0x00;
			inodes[inode_remove].DOUBLE_INDIRECT_BLOCKS[i] = 0x00;
		}

		// Nome do pai do arquivo a ser removido.
		string arquivoNome_pai = getFatherName(path);

		// Índice do inode do pai do arquivo a ser removido.
		int inode_pai = getInodeIndex(arquivoNome_pai, inodes, numInodes);

		// Decrementar o tamanho do pai do arquivo a ser removido.
		inodes[inode_pai].SIZE -= 1;

		// Lista de filhos do pai do arquivo a ser removido na região dos blocos.
		vector<vector<unsigned char>> filhos_pai;
		if (arquivoNome_pai == "/")
		{
			filhos_pai.push_back(blocos[0]);
			for (int i = 0; i < 2; i++)
			{
				if (inodes[inode_pai].DIRECT_BLOCKS[i] != 0x00)
				{
					filhos_pai.push_back(blocos[inodes[inode_pai].DIRECT_BLOCKS[i]]);
				}
			}
		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
				if (inodes[inode_pai].DIRECT_BLOCKS[i] != 0x00)
				{
					filhos_pai.push_back(blocos[inodes[inode_pai].DIRECT_BLOCKS[i]]);
				}
			}
		}

		// Se o inode removido não for o último filho do pai, mover o último filho para o lugar do removido.
		// Se B[k] = F e 0 ≤ k < P.SIZE -1 (ou seja, F não é o último filho de P), faça B[j] = B[j+1] para j=k, k+1, …, P.SIZE - 2

		vector<vector<unsigned char>> filhos_pai_aux(2, vector<unsigned char>(2, 0x00));

		// inicializar filhos_pai_aux com B1={0x01,0x02}, B2={0x07,0x0}.
		filhos_pai_aux[0][0] = 0x01;
		filhos_pai_aux[0][1] = 0x02;
		filhos_pai_aux[1][0] = 0x07;
		filhos_pai_aux[1][1] = 0x00;

		inode_remove = 0x01;

		// Se o inode removido não for o último filho do pai, mover o último filho para o lugar do removido.
		// Se B[k] = F e 0 ≤ k < P.SIZE -1 (ou seja, F não é o último filho de P), faça B[j] = B[j+1] para j=k, k+1, …, P.SIZE - 2

		// Remover o primeiro de três filhos do inode X cujos do blocos de dados
		// B1={0x01,0x02}, B2={0x07,0x0}.
		// Resultado: B1={0x02,0x07}, B2={0x07,0x0}
		

		// Liberar os blocos do arquivo a ser removido.
		for (int i = 0; i < blocosLiberar.size(); i++)
		{
			blocosUsados[blocosLiberar[i]] = false;
		}

		// Mapear os blocos usados para obter o bitmap.
		blocosUsados = mappingUsedBlocks(inodes, numBlocks, numInodes);

		for (int i = 0; i < numBlocks; i++)
		{
			int index = (int)floor(i / 8.0);

			if (blocosUsados[i] == true && i < 8)
			{
				bitMap[index] |= (1 << i);
			}
		}

		// Posicionar o ponteiro após o numero de inodes e escrever o bitmap e os inodes no arquivo.
		fseek(arquivo, 3, SEEK_SET);
		fwrite(&bitMap[0], sizeof(unsigned char), bitMapSize, arquivo);
		fwrite(&inodes[0], sizeof(INODE), numInodes, arquivo);

		// Pular a raiz e escrever os blocos tratados no arquivo.
		fseek(arquivo, 1, SEEK_CUR);
		for (int i = 0; i < numBlocks; i++)
		{
			fwrite(&blocos[i][0], sizeof(unsigned char), blockSize, arquivo);
		}
	}
	else
	{
		// Remover diretório
		// Se existir arquivo dentro do diretório retorna um erro.
	}

	fclose(arquivo);
}

/**
 * @brief Move um arquivo ou diretório em um sistema de arquivos que simula EXT3. O sistema já deve ter sido inicializado.
 * @param fsFileName arquivo que contém um sistema sistema de arquivos que simula EXT3.
 * @param oldPath caminho completo do arquivo ou diretório a ser movido.
 * @param newPath novo caminho completo do arquivo ou diretório.
 */
void move(string fsFileName, string oldPath, string newPath)
{
	FILE *arquivo = fopen(fsFileName.c_str(), "r+");
	if (arquivo == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}

	unsigned char blockSize, numBlocks, numInodes, raiz, bitmap;

	// Lendo o tamanho do bloco, número de blocos e número de inodes do arquivo.
	fseek(arquivo, 0, SEEK_SET);
	fread(&blockSize, sizeof(unsigned char), 1, arquivo);
	fread(&numBlocks, sizeof(unsigned char), 1, arquivo);
	fread(&numInodes, sizeof(unsigned char), 1, arquivo);

	// Quantidade de bytes do mapa de bits.
	unsigned char bitmapSize = getBitMapSize(numBlocks);
	fread(&bitmap, sizeof(unsigned char), 1, arquivo);

	// Espaço para o vetor de inodes
	vector<INODE> inodes(numInodes);
	fread(&inodes[0], sizeof(INODE), numInodes, arquivo);

	fread(&raiz, 1, 1, arquivo);

	// Espaço para o vetor de blocos
	vector<vector<unsigned char>> blocos(numBlocks, vector<unsigned char>(blockSize));
	for (int i = 0; i < numBlocks; i++)
	{
		fread(&blocos[i][0], sizeof(unsigned char), blockSize, arquivo);
	}

	// verificar se o pai do oldPath e do newPath são iguais
	string nomePai_newPath = "";
	string nomePai_oldPath = "";

	int ultimaBarra_new = newPath.find_last_of("/");
	int ultimaBarra_old = oldPath.find_last_of("/");

	for (int i = 1; i < ultimaBarra_new; i++)
	{
		nomePai_newPath += newPath[i];
	}
	if (nomePai_newPath == "")
	{
		nomePai_newPath = "/";
	}

	for (int i = 1; i < ultimaBarra_old; i++)
	{
		nomePai_oldPath += oldPath[i];
	}
	if (nomePai_oldPath == "")
	{
		nomePai_oldPath = "/";
	}

	if (nomePai_newPath == nomePai_oldPath)
	{
		// Pegar o nome do oldPath
		string nome_oldPath = oldPath.substr(oldPath.find_last_of("/") + 1);

		// Verificar qual é o inode do oldPath
		int inode_oldPath = 0;
		for (int i = 0; i < numInodes; i++)
		{
			if (inodes[i].NAME == nome_oldPath)
			{
				inode_oldPath = i;
				break;
			}
		}

		// Substituir o nome do oldPath pelo newPath
		string nome_newPath = newPath.substr(newPath.find_last_of("/") + 1);
		for (int i = 0; i < 10; i++)
		{
			if (i < nome_newPath.size())
			{
				inodes[inode_oldPath].NAME[i] = nome_newPath[i];
			}
			else
			{
				inodes[inode_oldPath].NAME[i] = 0x00;
			}
		}
	}

	// Atualiza o inode
	fseek(arquivo, 3 + bitmapSize, SEEK_SET);
	fwrite(&inodes[0], sizeof(INODE), numInodes, arquivo);

	fclose(arquivo);
}
