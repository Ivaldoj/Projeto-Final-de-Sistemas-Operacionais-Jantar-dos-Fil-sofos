# Etapa de build
FROM gcc:latest AS builder
WORKDIR /app
COPY . .
RUN make

# Etapa de runtime (imagem final menor)
FROM debian:stable-slim
WORKDIR /app
COPY --from=builder /app/filosofos /app/filosofos
CMD ["./filosofos"]
