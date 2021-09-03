import { ValidatorFn } from '@angular/forms';

export const validNumber: ValidatorFn = (control) => {
  // Before calling isNan, be sure value can't be empty or null.
  if (isNaN(Number(control.value))) {
    return {
      validNumber: {
        reason: 'invalid number',
        value: control.value
      }
    };
  }

  return null;
};
