---
layout: post
title:  "17 de Octubre"
date:   2024-10-17 20:30:00 - 23:30:00
categories: posts
---
Seguimos avanzando con la configuración de alertas en ThingsBoard. Hemos logrado establecer correctamente las notificaciones, de manera que, al crear o actualizar una alerta, se envíe un correo indicando si el sistema ha entrado en modo de resguardo o ha salido de él.

Además, resolvimos el problema con las alarmas que generaban una nueva notificación cada vez que se actualizaba un dato, aunque no hubiera un cambio de estado. Ahora, las alertas y correos se envían únicamente cuando hay una variación en el estado.

![Correos modo resguardo](https://github.com/SisCom-PI2-2024-2/proyecto-breeze-uy/blob/8df420c57c078730d3899b7eb6c6fafdd42dad43/docs/assets/Screenshot_20241017_230819_Gmail.jpg)

En cuanto al dispositivo físico, hemos impreso nuevas piezas con ligeras mejoras en el diseño para probar su integración con el resto de los componentes. También comenzamos las pruebas del modo de resguardo activando el servomotor conectado a la varilla.

