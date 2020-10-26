# Что дальше?

## Реализация panic
panic — одна из важнейших функций для предотвращения нежалательного поведения ОС. Она должна печатать текст ошибки и останавливать выполнение какого-либо кода ядра.

**Цель:** функция, на самом деле, уже реализована в [panic.h](panic.h), но сейчас она не выключает прерывания и грузит процессор бесконечным циклом. Перенесите её в `panic.c` и добавьте туда `cli/hlt`. Реализуйте также полезные asset-like макросы: [BUG/BUG_ON_NULL/etc](https://github.com/carzil/KeltOS/blob/f6429b65a1170c5350d5b88329bb24537f044135/include/kernel/panic.h#L8). *Для души (необязательно к выполнению):* добавьте ещё отображение EIP, на котором была вызывана паника через адрес возврата на стеке, а также состояние регистров процессора (как это делает Linux).

## Memory map
Скоро нам потребуется точно знать, где находится оперативнвая память, а где — memory-maped I/O. Для этого нужно разобраться, как получить информацию о расположении зарезервированных сегментах адресного пространства. Сделать это можно с помощью BIOS functions или с помощью информации, [предоставляемой](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Boot-information-format) по спецификации Multiboot. В любом случае будет полезна [статья](https://wiki.osdev.org/Detecting_Memory_(x86)) с OSDev.

**Цель:** во время загрузки ваша ОС должна печатать на экран зарезервированные и доступные участки памяти. Постарайтесь написать код так, чтобы его затем можно было переиспользовать — он потребуется вам для реализации страничной адресации.

## Валидация ACPI таблиц
По стандарту ACPI, ОС должна проверять целостность таблиц, считая их чек-сумму и сверяя с тем, что в таблице записано.

**Цель:** написать валидацию ACPI таблиц. Если таблица невалидна, следует вызывать панику.

## Калибровка APIC таймера
Сейчас в [apic.c](apic.c) указано рандомное число тиков, поэтому время между соседними прерываниями таймера неизвестно. В дальшнейшем нам потребуется знать, сколько времени прошло. Например, чтобы обрабатывать насильные переключения контекста раз в несколько миллисекунд, или пользовательские таймеры (`usleep/sleep`).

Для этого вам потребуется некоторый источник времени, который гарантированно отрабатывает за заданное время. Здесь есть несколько вариантов, можно использовать [Programmable Interval Timer](https://wiki.osdev.org/Programmable_Interval_Timer) или [подсмотреть](https://github.com/torvalds/linux/blob/7cf726a59435301046250c42131554d9ccc566b8/arch/x86/kernel/apic/apic.c#L825), как калибровка реализована в Linux.

**Цель:** настроить APIC timer так, чтобы время между соседними прерываниями составляло 1 миллисекунду. Реализация может быть любой.

## Обработка PS/2 клавиатур
Сейчас прерывания клавиатуры обрабатываются, но ничего полезного в них не происходит. Вообще, сейчас актуальны больше USB-клавиатуры, но мы пока остановимся на старых PS/2-клавиатурах. В этом задании вам самостоятельно предстоит разобраться, как работать с PS/2 и PS/2-клавиатурами.

Вам помогут:
* как всегда, [OSDev](https://wiki.osdev.org/PS/2_Keyboard);
* [код](https://github.com/mit-pdos/xv6-public) учебной xv6.

**Цель:** ваша ОС должна печатать символ, который пользователь ввёл с клавиатуры. Необходимо правильно поддерживать состояния клавиш Shift, Alt, Ctrl и CapsLock! *Для души:* научитесь мигать светодиодами на lock-клавишах :).