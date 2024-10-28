const Draco = require('./NativeDraco').default;

export function multiply(a: number, b: number): number {
  return Draco.multiply(a, b);
}
