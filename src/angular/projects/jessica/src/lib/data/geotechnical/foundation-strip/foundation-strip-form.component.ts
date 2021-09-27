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

import { FoundationStrip } from './foundation-strip';
/* eslint-enable max-len */

@Component({
  selector: 'lib-foundation-strip-form',
  templateUrl: './foundation-strip-form.component.html',
  styleUrls: ['./foundation-strip-form.component.css'],
  providers: [
    {
      provide: NG_VALUE_ACCESSOR,
      useExisting: forwardRef(() => FoundationStripFormComponent),
      multi: true
    },
    {
      provide: NG_VALIDATORS,
      useExisting: forwardRef(() => FoundationStripFormComponent),
      multi: true
    }
  ]
})
export class FoundationStripFormComponent
  implements ControlValueAccessor, Validator
{
  foundation: FormGroup;

  constructor(private fromBuilder: FormBuilder) {
    this.foundation = this.fromBuilder.group({
      width: [null, [Validators.required]]
    });
  }

  // ControlValueAccessor
  public onTouched!: () => void;
  writeValue(val: FoundationStrip<string>): void {
    val && this.foundation.setValue(val, { emitEvent: false });
  }
  registerOnChange(
    fn?: PartialObserver<{ [key: string]: string | undefined }>
  ): void {
    this.foundation.valueChanges.subscribe(fn);
  }
  registerOnTouched(fn: () => void): void {
    this.onTouched = fn;
  }
  setDisabledState?(isDisabled: boolean): void {
    isDisabled ? this.foundation.disable() : this.foundation.enable();
  }

  validate(): ValidationErrors | null {
    return this.foundation.valid
      ? null
      : {
          invalidForm: {
            valid: false,
            message: 'basicInfoForm fields are invalid'
          }
        };
  }
}
