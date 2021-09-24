import { Component } from '@angular/core';

import { cloneDeep } from 'lodash-es';

import {
  MeyerhofCalcService,
  MeyerhofCalc,
  MeyerhofCalcI18n,
  MeyerhofResult
} from 'jessica';
import { TranslateExService } from 'toolbox';

@Component({
  selector: 'app-main',
  templateUrl: './main.component.html',
  styleUrls: ['./main.component.css']
})
export class MainComponent {
  result!: MeyerhofResult;

  constructor(
    private calc: MeyerhofCalcService,
    public translateEx: TranslateExService
  ) {}

  compute(newItem: MeyerhofCalc<string>): void {
    const tt: MeyerhofCalc<string | number> = cloneDeep(newItem);

    this.translateEx.i18nStringToNumber(
      tt as unknown as Record<string, unknown>,
      new MeyerhofCalcI18n(),
      this.translateEx.numberParser
    );

    const ttt: MeyerhofCalc<number> = tt as MeyerhofCalc<number>;
    console.log(JSON.stringify(ttt));
    this.result = this.calc.compute(ttt.meyerhof);
  }
}
