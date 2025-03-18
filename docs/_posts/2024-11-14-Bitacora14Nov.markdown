---
layout: post
title:  "14 de Noviembre"
date:   2024-11-14 20:30:00 -0300
categories: posts
---

En el dia de hoy comenzamos con el desarrollo del Sleep mode. Se estudio segun la placa usada los modos de sleep disponibles. Se va a implementar un light sleep mode en conjunto con interrupciones de modo de que
se pueda seguir midiendo la direccion del viento (este aspecto es crucial para no perder la referencia del viento y quedar descalibrado).

Al final de la jornada se llego a aproximadamente el 85% de la solucion de sleep mode. Quedaron errores para corregir en cuanto al uso de las interrupciones y la reanudacion del codigo.
