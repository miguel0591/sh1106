# DRIVER OLED SH1106 + PRUEBAS FORMALES

## Uso del repositorio

Este repositorio utiliza [pre-commit](https://pre-commit.com) para validaciones de formato. Para trabajar con el mismo usted debería tener instalado:

1. pre-commit (https://pre-commit.com/#install)

Después de clonar el repositorio usted debería ejecutar el siguiente comando:

```
pre-commit install
```

Para generar la documentación del proyecto se utiliza el siguiente comando (crear la carpeta build/doc o modificar la ruta):

```
doxygen

```

El repositorio usa ceedling como herramienta de automatizacion de las pruebas.
para correlo ejecutar el comando:

```
ceedling

```

## License

This work is distributed under the terms of the [MIT](https://spdx.org/licenses/MIT.html) license.
