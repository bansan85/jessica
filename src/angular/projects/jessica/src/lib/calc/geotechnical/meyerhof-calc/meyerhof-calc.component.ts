/* eslint-disable max-len */
import {
  Component,
  EventEmitter,
  OnDestroy,
  OnInit,
  Output
} from '@angular/core';
import { FormBuilder, FormGroup } from '@angular/forms';
import { Subscription } from 'rxjs';
import { filter } from 'rxjs/operators';

import { MeyerhofCalc } from './meyerhof-calc';
/* eslint-enable max-len */

@Component({
  selector: 'lib-meyerhof-calc',
  templateUrl: './meyerhof-calc.component.html',
  styleUrls: ['./meyerhof-calc.component.css']
})
export class MeyerhofCalcComponent implements OnInit, OnDestroy {
  form: FormGroup;
  @Output() changeEvent = new EventEmitter<MeyerhofCalc>();
  private obs$!: Subscription;
  constructor(private fromBuilder: FormBuilder) {
    this.form = this.fromBuilder.group({
      meyerhof: ['']
    });
  }

  ngOnInit(): void {
    this.obs$ = this.form.valueChanges
      .pipe(filter(() => this.form.valid))
      .subscribe((a: MeyerhofCalc) => {
        this.changeEvent.emit(a);
      });
  }

  ngOnDestroy(): void {
    this.obs$?.unsubscribe();
  }

  setDisabledState?(isDisabled: boolean): void {
    isDisabled ? this.form.disable() : this.form.enable();
  }
}
