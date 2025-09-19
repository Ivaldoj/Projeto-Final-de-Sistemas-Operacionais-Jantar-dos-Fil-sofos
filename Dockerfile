FROM ubuntu:24.04

# Instala dependências
RUN apt-get update -y && apt-get install -y build-essential

# Cria diretório de trabalho
WORKDIR /app

# Copia tudo
COPY . .

# Compila com make
RUN make

# Comando padrão
CMD ["./philosophers"]
