import { Component } from '@angular/core';

import { cloneDeep } from 'lodash-es';

import {
  MeyerhofCalcService,
  MeyerhofCalc,
  MeyerhofCalcI18n,
  MeyerhofResult
} from 'jessica';
import { TranslateExService } from 'toolbox';
import { LangChangeEvent, TranslateService } from '@ngx-translate/core';
import { FormBuilder, FormGroup } from '@angular/forms';

@Component({
  selector: 'app-main',
  templateUrl: './main.component.html',
  styleUrls: ['./main.component.css']
})
export class MainComponent {
  result!: MeyerhofResult;
  form: FormGroup;
  private _lang?: string;

  constructor(
    private calc: MeyerhofCalcService,
    public translate: TranslateService,
    public translateEx: TranslateExService,
    private fromBuilder: FormBuilder
  ) {
    this.form = this.fromBuilder.group({
      child: ['']
    });
    this.translate.onLangChange
      // Convert from this.translateEx.language to event.lang
      .subscribe((event: LangChangeEvent) => {
        if (this._lang === undefined) {
          this._lang = event.lang;
          return;
        }
        if (this.form.controls.child.value === '') {
          return;
        }
        const formCalcStr: MeyerhofCalc<string | number> = cloneDeep(
          this.form.controls.child.value
        );
        this.translateEx.i18nStringChangeLocale(
          formCalcStr as unknown as Record<string, unknown>,
          new MeyerhofCalcI18n(),
          this._lang,
          event.lang
        );
        const formCalcNum: MeyerhofCalc<number> =
          formCalcStr as MeyerhofCalc<number>;
        this.form.controls.child.setValue(formCalcNum);
        this._lang = event.lang;
      });
  }

  compute(newItem: MeyerhofCalc<string>): void {
    const formCalcStr: MeyerhofCalc<string | number> = cloneDeep(newItem);

    this.translateEx.i18nStringToNumber(
      formCalcStr as unknown as Record<string, unknown>,
      new MeyerhofCalcI18n(),
      this.translateEx.numberParser
    );

    const formCalcNum: MeyerhofCalc<number> =
      formCalcStr as MeyerhofCalc<number>;
    this.result = this.calc.compute(formCalcNum.meyerhof);
  }
}
