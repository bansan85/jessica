/* eslint-disable max-len */
import { I18nForm } from 'toolbox';

import {
  MeyerhofForm,
  MeyerhofFormI18n
} from '../../../data/geotechnical/meyerhof/meyerhof-form';
/* eslint-enable max-len */

export interface MeyerhofCalc<T extends number | string> {
  meyerhof: MeyerhofForm<T>;
}

export class MeyerhofCalcI18n implements I18nForm {
  public meyerhof: MeyerhofFormI18n = new MeyerhofFormI18n();
  public child: string[] = ['meyerhof'];
}
