//Ejemplo: 65 0 2 0 ffff ffff ffff
//
//  65: This message is a touch event
//  0: The page ID is 0
//  2: Component ID (the number of the first button in the Editor)
//  0: Type of event. A "0" means a Release event, A "1" is a Press event
//  FFFF FFFF FFFF: The end of message pattern
//
//
//b0 -> I.Entrada:        65 1 1 1 ffff ffff ffff
//b1 -> I.Receso:         65 1 2 1 ffff ffff ffff
//b2 -> I.Almuerzo:       65 1 3 1 ffff ffff ffff
//b3 -> Salida:           65 1 4 1 ffff ffff ffff
//b4 -> F.Receso:         65 1 5 1 ffff ffff ffff
//b5 -> F.Almuerzo:       65 1 6 1 ffff ffff ffff

