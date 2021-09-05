/* eslint-disable max-len */
import { Component, forwardRef } from '@angular/core';
import {
  ControlValueAccessor,
  FormGroup,
  Validators,
  NG_VALUE_ACCESSOR,
  NG_VALIDATORS,
  ValidationErrors,
  FormBuilder,
  Validator
} from '@angular/forms';
import { PartialObserver } from 'rxjs';

import { validNumber } from 'toolbox';

import { VerticalEccentric } from './vertical-eccentric';
/* eslint-enable max-len */

@Component({
  selector: 'lib-vertical-eccentric-form',
  templateUrl: './vertical-eccentric-form.component.html',
  styleUrls: ['./vertical-eccentric-form.component.css'],
  providers: [
    {
      provide: NG_VALUE_ACCESSOR,
      useExisting: forwardRef(() => VerticalEccentricFormComponent),
      multi: true
    },
    {
      provide: NG_VALIDATORS,
      useExisting: forwardRef(() => VerticalEccentricFormComponent),
      multi: true
    }
  ]
})
export class VerticalEccentricFormComponent
  implements ControlValueAccessor, Validator
{
  load: FormGroup;

  constructor(private fromBuilder: FormBuilder) {
    this.load = this.fromBuilder.group({
      load: [null, [Validators.required, validNumber]],
      eccentric: [null, [Validators.required, validNumber]]
    });
  }

  // ControlValueAccessor
  public onTouched!: () => void;

  writeValue(val: VerticalEccentric): void {
    val && this.load.setValue(val, { emitEvent: false });
  }
  registerOnChange(
    fn?: PartialObserver<{ [key: string]: string | undefined }>
  ): void {
    this.load.valueChanges.subscribe(fn);
  }
  registerOnTouched(fn: () => void): void {
    this.onTouched = fn;
  }
  setDisabledState?(isDisabled: boolean): void {
    isDisabled ? this.load.disable() : this.load.enable();
  }

  validate(): ValidationErrors | null {
    return this.load.valid
      ? null
      : {
          invalidForm: {
            valid: false,
            message: 'basicInfoForm fields are invalid'
          }
        };
  }
}
