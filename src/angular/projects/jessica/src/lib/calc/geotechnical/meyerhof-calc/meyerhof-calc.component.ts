/* eslint-disable max-len */
import { Component, OnDestroy, OnInit } from '@angular/core';
import { FormBuilder, FormGroup } from '@angular/forms';
import { Subscription } from 'rxjs';
import { filter } from 'rxjs/operators';

import { TranslateExService } from 'toolbox';

import { MeyerhofForm } from '../../../data/geotechnical/meyerhof/meyerhof-form';
import { MeyerhofCalc } from './meyerhof-calc';
import { MeyerhofCalcService } from './meyerhof-calc.service';
import { MeyerhofResult } from './meyerhof-result';
/* eslint-enable max-len */

@Component({
  selector: 'lib-meyerhof-calc',
  templateUrl: './meyerhof-calc.component.html',
  styleUrls: ['./meyerhof-calc.component.css']
})
export class MeyerhofCalcComponent implements OnInit, OnDestroy {
  form: FormGroup;
  result!: MeyerhofResult;
  private obs$!: Subscription;
  constructor(
    private calc: MeyerhofCalcService,
    private fromBuilder: FormBuilder,
    public translateEx: TranslateExService
  ) {
    this.form = this.fromBuilder.group({
      meyerhof: ['']
    });
  }

  compute(newItem: MeyerhofForm): void {
    this.result = this.calc.compute(newItem);
  }

  ngOnInit(): void {
    this.obs$ = this.form.valueChanges
      .pipe(filter(() => this.form.valid))
      .subscribe((a: MeyerhofCalc) => {
        this.compute(a.meyerhof);
      });
  }

  ngOnDestroy(): void {
    this.obs$?.unsubscribe();
  }

  setDisabledState?(isDisabled: boolean): void {
    isDisabled ? this.form.disable() : this.form.enable();
  }
}
