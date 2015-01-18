/** @mainpage Voto Electrónico
*
* @Autores Grupo Furlong:
* - Facundo Rossi (padrón 86.707 - frossi85@gmail.com)
* - Leandro Miguenz (padrón 90.649 - leandro.v.059@gmail.com)
* - Martín Lucero (padrón 89.630 - don_pipa182@yahoo.com)
* - Miguel Torres (padrón 91.396 - mat1204@hotmail.com)\n
* <hr>
*
* @section primera Primera Entrega
*
*
* @section imple Implementación
*
* Debido a que la problemática que presentan las distintas entidades del dominio son diferentes, se utilizaran distintos tipo de organizaciones
* y estructuras de datos de acuerdo a la entidad.
* \n\n
*
* Aparte del ID de cada entidad, se introducirá uno numérico (entero). De esta manera se lograra una mejor performance en búsquedas, y una implementación
* mas sencilla de los algoritmos de las estructuras de archivos.\n
* Además solucionaría problemas de tamaño de almacenamiento dados por almacenar entidades enteras dentro de otra entidad que las referencia, como es el
* caso de votante que contienen elecciones indefinidas.
* \n\n
*
* Tanto el ID que agregamos nosotros, como el identificador de la clase (x ej en votante el DNI), no podrán tener duplicados. Por eso al realizar cualquier
* alta o modificación, se deberá comprobar que sean únicos.
* \n\n
*
* Si se cambia el identificador (o parte de él) de una instancia particular de cualquier tipo de entidad, antes de eso se deberán actualizar los índices
* que relacionen la clave autonumerica con el identificador de la instancia, modificando este ultimo con el nuevo valor. Lo mismo si se borra una entidad,
* primero se actualizan los índices y luego se elimina.
*
*
*
* @section orga Organización
*
* Nueva definición de las entidades:\n\n
*
* - Distrito (id, (distrito)i)
* - Votante (id, (DNI)i, NombreyApellido, clave, domicilio, (idDistrito)ie, ((idEleccion)ie)*)
* - Eleccion (id, (fecha, (idCargo)ie)i, ((idDistrito)ie)+)
* - Lista (id, ((idEleccion)ie, nombre)i)
* - Candidato (id, ((idLista)ie, (idVotante)ie, (idCargo)ie)i)
* - Cargo (id, (cargo)i, (cargo)*)
* - Conteo (((idLista)ie, (idDistrito)ie, (idEleccion)ie)i, cantidad)
* - Administrador ((usuario)i, clave) \n
* (Conteo no necesita id numérico ya que ninguna otra entidad tiene referencias a ella. Relacionamos directamente
* la clave con el offset en el archivo de conteos. Lo mismo para Administrador)
*
* Estructuras utilizadas:
*
* Estructuras del TP (cuando se usa la palabra id, se refiere a la clave autonumerica):
*
* 1- Hash Votante/id =  clave: DNI (pasar de int a string), dato: id del votante \n
* 2- Hash idVotante/offset =  clave: id_votante, dato: offset del votante en el archivo \n
*
* 3- Hash Distrito/id = clave: nombreDistrito (string), dato: id del distrito\n
* 4- Hash idDistrito/offset =  clave: id_distrito, dato: offset del distrito en el archivo \n
*
* 5- Hash Cargo/id = clave: cargoPrincipal (string), dato: id del cargo\n
* 6- Hash idCargo/offset: clave: id_cargo, dato: offset del cargo en el archivo \n
*
* 7- Arbol Candidato/id = clave: fecha$cargoPpal$nombreLista$DNI(string). dato: id del candidato\n
* 8- Hash idCandidato/offset = clave: id_candidato, dato: offset del candidato en el archivo \n
*
* 9- Arbol Lista/id = clave: fecha$cargoPpal$nombreLista. dato: id de la lista. (el $ es el símbolo que usamos para concatenar las cadenas)\n
* 10- Hash idLista/offset = clave: id_lista, dato: offset de la lista en el archivo \n
*
* 11- Arbol Eleccion/id = clave: fecha$cargoPrincipal . dato: id_eleccion.\n
* 12- Hash idEleccion/offset = clave: id_eleccion . dato: offset en el archivo \n
*
* 13- ARBOL REPORTE DISTRITO = clave: nom_distrito$fecha$cargoPrincipal$nom_lista. dato: offset del conteo en el archivo.\n
* 14- ARBOL REPORTE LISTA = clave: fecha$cargoPrincipal$nom_lista$nom_distrito. dato: offset del conteo en el archivo.\n
* 15- ARBOL REPORTE ELECCION = clave: fecha$cargoPrincipal$nom_distrito$nom_lista. dato: offset del conteo en el archivo.\n
*
*
* Todas las entidades se almacenan en archivos de registros de tamaño variable.\n
* En el caso de aquellas que implementan id numérico, se utiliza un hash para relacionar ese id con su correspondiente clave.\n
* Es decir que primero se le pasa su id a dicho hash para obtener la clave, y luego se le pasa esa clave al hash/árbol índice de
* esa entidad, para finalmente obtener el offset del registro en su archivo de registros variables.
* \n\n
*
* Para las entidades que tienen más de un campo como identificador (Elección, Lista y Candidato), se utilizan índices B+, para
* aprovechar la posibilidad de hacer búsquedas aproximadas que brinda esa estructura.
* Distrito, Votante y Cargo usan índices de dispersión extensible modular.
*
* En el caso particular de Conteo, tiene tres índices B+, para posibilitar la recuperación de conteos por lista, distrito o
* elección, y usarlos para imprimir el reporte de resultados en base a alguno de esos tres criterios.\n\n
*
* Los conteos se crean todos con cantidad 0 cuando se da de alta una lista.
* Cuando se confirma el voto del usuario, a partir de su distrito y de la lista que votó se obtiene el conteo correspondiente
* (buscándolo en el árbol) y se incrementa su cantidad de votos.
* <hr>
*
* @section res Resumen:
*
* - Los datos (las entidades propiamente dichas) se guardaran en archivos de registros variables o fijos (según de qué entidad se trate).
* - La búsqueda de estos datos se realizará mediante índices implementados con un hash extensible y un árbol B+, que van a guardar un
*   par (clave, offset en archivo).
* - El flujo del programa tendría esta forma:\n
* 1- el administrador ingresa una clave.\n
* 2- el hash/árbol usado cómo índice recibe ese id, y devuelve un offset.\n
* 3- se accede a ese offset para obtener el registro deseado.\n
* 4- el administrador modifica el registro.\n
* 5 - Se persisten los cambios, y si es necesario, se modifica el offset en el hash/árbol índice.
* \n\n
* (Para altas, bajas y votaciones la estructura es similar).\n
* <hr>
* @section notas Sobre el formato de las claves
*
* Aclaraciones:\n\n
*
* 1 - Cuando se guarda una clave en el hash o arbol, antes se llama a Utilidaes::formatearClave(clave)
* para reemplazar los espacios por '&'.\n
*
* 2 - Para las claves con fechas, tambien antes de guardarlas se le hace a la fecha Utilidades::indexarFecha(fecha)
* para llevarla al formato aaaammdd (la fecha original ingresada por el administrador debe ser del formato ddmmaaaa o dd/mm/aaaa. No es
* valido un formato d/m/aaaa, es decir, debe completarse con ceros si es menor a 10.\n
* 
* 3 - También cuando una clave es compuesta, se concatenan los strings con '$'.\n\n
*
* Ejemplo ilustrativo: el administrador habia insertado una lista con fecha "20/10/2011", cargo "Diputado Cordoba", nombre "UCR".
* El ABMentidades::altaLista inserta la clave en la forma "20111020$Diputado&Cordoba$UCR".\n\n
*
* - Distrito: "nombre_distrito" | Ejemplo: "Lanus"\n
* - Cargo: "nombre_cargo" | Ejemplo: "Intendente&Lanus"\n
* - Eleccion: "fecha$cargo" | Ejemplo: "20111020$Intendente&Lanus"\n
* - Lista: "fecha$cargo$nombreLista" | Ejemplo: "20111020$Intendente&Lanus$UCR"\n
* - Votante: "DNI" | Ejemplo: "34094150"\n
* - Candidato: "fecha$cargo$nombreLista$DNI" | Ejemplo: "20111020$Intendente&Lanus$UCR$34094150"\n
*
* <hr>
* @section ind Índices
*
* @section hash Hash:
*
* - Función de hashing utilizada:\n\n
*
*@code unsigned int hash_extensible::funcion_hashing(RegistroIndice *registro){
*
*	string clave = registro->getClave();
*	unsigned int tam_tabla = obtener_tamanio_tabla();
*
*	int valor = 0;
*
*   for( int i = 0; i < clave.length() ; i++ )
*        valor = 37 * valor + clave.at( i );
*
*    valor %= tam_tabla;
*    if( valor < 0 )
*        valor += tam_tabla;
*
*    return valor + 1;
} @endcode
*
* En un principio usamos una función mucho más simple, que obtenía el valor ASCII de los caracteres de la clave, los sumaba, y luego calculaba
* el módulo de ese valor con el tamaño de la tabla de dispersión. Esa función dio malos resultados al intentar guardar grandes cantidades de registros
* en el hash, volviendo demasiado lenta y casi imposible la inserción de más de 500 registros, aproximadamente, debido a la cantidad de colisiones que se
* producían.
* \n\n
*
* Con la actual función de hashing los resultados fueron mucho mejores, y pudimos insertar hasta 1 millón de registros en un tiempo de ejecución razonable.
*
* - Tamaño de bloques:\n\n
*
* Para los bloques del hash elegimos un tamaño de 512 bytes, porque es un múltiplo de 512*2^n, al igual que los bloques en que está organizaod el disco,
* lo que permite aprovechar mejor el buffering del sistema.\n
* Además consideramos que es un tamaño adecuado a nuestras necesidades, ya que los bloques del hash almacenan objetos de la clase RegistroIndice,
* que una vez persistidos con sus metadatos, ocupan un estimado de entre 13 y 18 bytes cada uno, y por lo tanto en cada bloque caben más de 30 registros.
*
* @section arbol Árbol B+:
*
* - Registros:\n\n
*
* El registro consta de 3 atributos, una clave(string), un offset u id(long) y un atributo denominado link (long) que es como indica su nombre es un
* link directo al registro indmediatamente mayor a él. Este último atributo se implementa para realizar el recorrido secuencial desde un registro
* en vez de hacerlo desde un nodo.\n
* Los unicos campos variables dentro los registros del árbol son las claves, que son del tipo string. Para este tipo de dato se graba el largo
* de la clave como un entero (int), sin contar el '/0'. Seguido de eso se procede a guardar cada caracter del string sin tener en cuenta
* el caracter de finalización.
* \n\n
*
* - Nodos:\n\n
*
* El nodo posee como atributos su tamaño máximo, la cantidad de registros que puede albergar, los registros mismos, una dirección dentro
* del archivo del árbol, una dirección al padre, una al nodo inmediatamente menor y un flag que es utilizado para indicar si el nodo fue
* modificado después de leerlo del archivo del árbol. Este último es utilizado para implementar una política de caché tal que al leer un
* nodo, se guarda en memoria y mientras no se supere la cantidad máxima de nodos en memoria seteados por el árbol no se impacten los cambios,
* ni se lea un nodo desde el disco si está en cachá. Además se provee de un mecanismo interno que permite obligar a impactar los cambios y
* liberar. Este es útil en casos como por ejemplo al cerrar o eliminar el árbol.\n
* Como se describió antes, el encargado de saber la ubicación de un registro mayor, pertenece al registro mismo, por lo que no se incluye
* en el nodo el link al nodo hermano.\n
* Para la búsqueda de registros dentro de un nodo en memoria se utilizó búsqueda binaria, la cual en memoria es muy eficiente debido a que
* los registros estan ordenados.\n\n
*
*  - Reestructuración por overflow y underflow:\n\n
*
*  El split y la combinación de nodos se realiza a nivel del árbol. Es decir que no es un método implementado en la clase Nodo.\n
*  Para la combinación, se fusionan los dos nodos sobre el nodo izquierdo y se libera el nodo derecho. Esta operación de combinar nodo
*  se utiliza por el rebalancear nodo, que va rebalancenado el árbol desde el primer nodo en el que hay que combinar un nodo hasta llegar
*  a la raíz o que no halla que combinar.\n
*  El split es similar, se busca el nodo donde debería insertarse el registro; si necesita ser partido, se parte y se extiende la partición
*  desde el nodo hacia arriba o sus menores.
*
* <hr>
* @section bugs Bugs conocidos
*
* Hash: \n\n
*
* - Quedaron algunas pérdidas de memoria sin resolver. Según las pruebas hechas, insertar 1 millón de registros de una vez consume 3 Gb de RAM.
* Sin embargo, para inserciones de menos de 300.000 elementos, la pérdida de memoria es mucho menor, y se pueden realizar sin problemas en una
* computadora con 2 o más Gb de RAM. Independientemente de la memoria del sistema, se puede guardar cualquier cantidad de registros, siempre
* y cuando no se los guarde a todos en una sola corrida (es decir, guardando algunos, cerrando el programa y guardando los que faltan).
* \n\n
*
* Votación: \n\n
*
* - El método getListaPorEleccion() está devolviendo las listas de la elección que recibe por parámetro y de la siguiente elección en fecha.
* Puede deberse a una inconsistencia de datos de la carga inicial, no se llegó a corroborar.\n\n
*
* \n\n\n
* <hr>
* Aclaración importante:\n\n
*
*  No es posible cambiar la fecha ni el cargo principal de una elección, ni agregarle cargos secundarios.
*  Si se quieren agregar, debe hacerse desde el modificar de Cargo. Es decir, la opción de agregar cargos secundarios
*  no debería estar en el modificar de Elección, sino sólo en el de Cargo, pero por cuestiones de tiempo no llegamos a
*  eliminar dicha opción del menu.\n
*  Por otra parte, el modificar de Elección sí permite agregar o quitar distritos.
*
*
*  <hr>
* @section segunda Segunda Entrega
*
* Para esta entrega se agregó funcionalidad al trabajo práctico para realizar la encriptación de la información de votantes
* y administradores usando RSA, y la encriptación de los reportes mediante el algoritmo de Vigenere.\n
* El algoritmo RSA genera las claves pública y privada siguiendo el procedimiento de multiplicación de números primos visto
* en clase.\n
* También se agregaron opciones para romper dichos criptogramas usando factorización de números primos y el
* algoritmo de Kasiski, respectivamente.\n
* A excepción de la utilización de RSA, que es transparente al usuario, el resto de la funcionalidad se agregó en forma de nuevas
* opciones en el menú del administrador, o que se acceden mediante ciertos argumentos por línea de comandos. Su uso se explica a
* continuación.
*
* @section uso Modos de uso
*
* @section carga Carga inicial:
*
* Para tener datos cargados en el programa, de manera tal que se puedan probar todas sus características sin tener que crear entidades
* ni votar manualmente, se pueden usar las opciones de carga inicial y votante automático de la siguiente manera: \n\n
*
* ./VotoElectronico -c \n
* ./VotoElectronico -v \n\n
*
*
* @section romperRSA Romper RSA:
*
* Para probar el algoritmo de factorización, se debe ejecutar el programa por línea de comandos de la siguiente forma: \n\n
*
* ./VotoElectronico --romperRSA \n\n
*
* Por pantalla se mostrarán los pasos seguidos por el algoritmo y sus resultados. Para romper la encriptación de RSA se utiliza
* factorización de números primos (ver clase Factorizacion), aplicando divisiones sucesivas y el método de Fermat. El método
* encargado de esta tarea es RSA::romper.
*
* @section encriptar Encriptación/rotura de reportes con Vigenère y Kasiski:
*
* Antes de empezar a trabajar con los reportes, estos deben ser generados. Para hacerlo, hay que ingresar como administrador
* (recordar que por defecto se puede usar la cuenta con usuario y contraseña "tp"), en el menú seleccionar la opción "R"
* (Informes de resultados), y en el submenú que aparece, elegir alguno de los 3 tipos de reportes para ver. El reporte elegido
* se mostrará por pantalla, y además se guardará en un archivo de texto en la carpeta src/Reportes.
*
* Una vez que haya algún reporte, hay que acceder como administrador, y en el menú principal seleccionar la opción "G"
* (Encriptar/Desencriptar/Romper Reportes). En el submenú que aparece figuran tres opciones: \n\n
*
* - Encriptar: se pide una clave de encriptación, la ruta del archivo a encriptar, y la ruta donde se desea guardar el criptograma (que puede ser la misma).
* Vale la pena aclarar que cuando se ingresa la ruta, se debe considerar que "./" 'apunta' al directorio del programa ejecutable.
* Como los reportes se encuentran en una carpeta "Reportes" dentro del directorio donde se encuentra el ejecutable, entonces
* la ruta sería "./Reportes/ReporteDistrito", por ejemplo.
* Cuando se pide el archivo sin encriptar para encriptar (o cuando se pide el encriptado para desencriptar), este debe existir.
* De lo contrario, se mostrará un mensaje de error. Cuando se pide la ruta para generar el archivo encriptado (o generear el
* desencriptado), en caso de existir ya un archivo con el nombre que se ingresa, este se sobreescribe.
* La clave puede tener sólo caracteres ascii de 7 bits
* (es decir, no ascii extendido). Esto se debe a que esta implementación del algoritmo realiza las operaciones en módulo 127 (usa una
* tabla de 127 símbolos para los reemplazos).
* Ya que la idea de la encriptación en este caso es aplicarla cuando se quiere mandar el reporte a alguien, y para facilitar la
* observación del funcionamiento de los algoritmos, se decidió que en principio el reporte se guarde como texto plano sin encriptar.
*
* - Desencriptrar: el funcionamiento es análogo al de la opción anterior.
*
* - Romper: también se pide una ruta de origen y una de destino. Si el archivo de origen (el criptograma) no existiese, se mostrará
* un mensaje de error. En la primera línea del archivo generado se muestra cuál
* era la clave de encriptación. Puede pasar que para un determinado criptograma, el método no sea efectivo y el archivo
* obtenido resulte total o parcialmente ilegible. En la entrega se incluye el criptograma de ejemplo "EjemploParaKasiski"
* en la carpeta Reportes, para que se pueda probar este algoritmo en un caso en el que sabemos que cumple su cometido.
* Al igual que Vigenère, la implementación de este algoritmo trabaja con los primeros 127 ascii.
* Básicamente, el funcionamiento es el siguiente: se aplica el método de Kasiski para determinar la longitud de la clave. Luego
* se alinea el criptograma en n columnas, siendo n dicha longitud, y en cada columna se hace un análisis de frecuencias. Se asume
* que el caracter que aparezca más veces se corresponde con el espacio (' '). También se generan posibles claves probando reemplazar ese
* caracter por 'e' y 'a', en ese orden, ya que son los que aparecen con más frecuencia en un texto en castellano. Se realiza la resta de
* los valores ascii de esos caracteres, y se obtiene el primer caracter de la clave. Repitiendo el proceso con las demás columnas se obtiene
* el resto de la clave. Al terminar este proceso, se tienen 3 claves posibles (una por cada reemplazo probado). Finalmente se usa cada clave
* para desencriptar el texto (se generan 3 archivos "desencriptados"). El algoritmo recorre esos tres archivos, busca las palabras clave (eleccion,
* lista, distrito, etc.), y deja sólo aquel en el que estas palabras aparezcan más veces, borrando los demás.
*
*/
