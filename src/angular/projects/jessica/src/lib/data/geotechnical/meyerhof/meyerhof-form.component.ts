import { Component, forwardRef, OnDestroy, OnInit } from '@angular/core';
import {
  ControlValueAccessor,
  FormBuilder,
  FormGroup,
  NG_VALIDATORS,
  NG_VALUE_ACCESSOR,
  ValidationErrors,
  Validator
} from '@angular/forms';
import { PartialObserver, Subscription } from 'rxjs';
import { debounceTime, filter } from 'rxjs/operators';

import { MeyerhofForm } from './meyerhof-form';

@Component({
  selector: 'lib-meyerhof-form',
  templateUrl: './meyerhof-form.component.html',
  styleUrls: ['./meyerhof-form.component.css'],
  providers: [
    {
      provide: NG_VALUE_ACCESSOR,
      useExisting: forwardRef(() => MeyerhofFormComponent),
      multi: true
    },
    {
      provide: NG_VALIDATORS,
      useExisting: forwardRef(() => MeyerhofFormComponent),
      multi: true
    }
  ]
})
export class MeyerhofFormComponent
  implements OnInit, OnDestroy, ControlValueAccessor, Validator
{
  form: FormGroup;

  private obs$!: Subscription;

  constructor(private fromBuilder: FormBuilder) {
    this.form = this.fromBuilder.group({
      foundation: [''],
      load: ['']
    });
  }

  ngOnInit(): void {
    this.obs$ = this.form.valueChanges
      .pipe(filter(() => this.form.valid))
      .pipe(debounceTime(200))
      .subscribe((a: MeyerhofForm<string>) => {
        this.writeValue(a);
      });
  }

  ngOnDestroy(): void {
    this.obs$?.unsubscribe();
  }

  // ControlValueAccessor
  public onTouched!: () => void;
  writeValue(val: MeyerhofForm<string>): void {
    val && this.form.setValue(val, { emitEvent: false });
  }
  registerOnChange(
    fn?: PartialObserver<{ [key: string]: string | undefined }>
  ): void {
    this.form.valueChanges.subscribe(fn);
  }
  registerOnTouched(fn: () => void): void {
    this.onTouched = fn;
  }
  setDisabledState?(isDisabled: boolean): void {
    isDisabled ? this.form.disable() : this.form.enable();
  }

  validate(): ValidationErrors | null {
    if (this.form.controls.foundation.valid && this.form.controls.load.valid) {
      return null;
    } else {
      return {
        invalidForm: {
          valid: false,
          message: 'MeyerhofForm fields are invalid'
        }
      };
    }
  }
}
