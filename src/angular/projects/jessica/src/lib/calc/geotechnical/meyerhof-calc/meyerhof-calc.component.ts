/* eslint-disable max-len */
import {
  Component,
  EventEmitter,
  forwardRef,
  OnDestroy,
  OnInit,
  Output
} from '@angular/core';
import {
  ControlValueAccessor,
  FormBuilder,
  FormGroup,
  NG_VALUE_ACCESSOR
} from '@angular/forms';
import { PartialObserver, Subscription } from 'rxjs';
import { filter } from 'rxjs/operators';

import { MeyerhofCalc } from './meyerhof-calc';
/* eslint-enable max-len */

@Component({
  selector: 'lib-meyerhof-calc',
  templateUrl: './meyerhof-calc.component.html',
  styleUrls: ['./meyerhof-calc.component.css'],
  providers: [
    {
      provide: NG_VALUE_ACCESSOR,
      useExisting: forwardRef(() => MeyerhofCalcComponent),
      multi: true
    }
  ]
})
export class MeyerhofCalcComponent
  implements ControlValueAccessor, OnInit, OnDestroy
{
  form: FormGroup;
  @Output() changeEvent = new EventEmitter<MeyerhofCalc<string>>();
  private obs$!: Subscription;
  constructor(private fromBuilder: FormBuilder) {
    this.form = this.fromBuilder.group({
      meyerhof: ['']
    });
  }

  ngOnInit(): void {
    this.obs$ = this.form.valueChanges
      .pipe(filter(() => this.form.valid))
      .subscribe((a: MeyerhofCalc<string>) => {
        this.changeEvent.emit(a);
      });
  }

  ngOnDestroy(): void {
    this.obs$?.unsubscribe();
  }

  // ControlValueAccessor
  public onTouched!: () => void;
  writeValue(val: MeyerhofCalc<string>): void {
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
}
