---
layout: post
title:  "10 de Octubre"
date:   2024-10-10 20:30:00 - 23:30:00
categories: posts
---

Continuamos trabajando en la configuración de las alertas en ThingsBoard, siguiendo la prioridad establecida previamente.

Notificaciones de Alertas:
Configuramos el sistema para que envíe correos electrónicos cuando se crean o actualizan alertas.
Verificamos que los correos electrónicos se enviaran adecuadamente en función del estado del sistema (modo de resguardo activado o desactivado).
Solución de Problemas con las Alarmas:

Identificamos un problema en el cual se generaban notificaciones innecesarias cada vez que se actualizaban datos, incluso si no había un cambio de estado. 

Para resolver esto:
Modificamos la lógica de las alertas para que las notificaciones y correos se enviaran únicamente cuando hubiera una variación real en el estado del sistema.


