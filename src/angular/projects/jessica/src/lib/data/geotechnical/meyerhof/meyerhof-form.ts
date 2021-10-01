/* eslint-disable max-len */
import { I18nForm } from 'toolbox';

import {
  FoundationStrip,
  FoundationStripI18n
} from '../foundation-strip/foundation-strip';
import {
  VerticalEccentric,
  VerticalEccentricI18n
} from '../../load/vertical-eccentric/vertical-eccentric';
/* eslint-enable max-len */

export interface MeyerhofForm<T extends number | string> {
  foundation: FoundationStrip<T>;
  load: VerticalEccentric<T>;
}

export class MeyerhofFormI18n implements I18nForm {
  public foundation: FoundationStripI18n = new FoundationStripI18n();
  public load: VerticalEccentricI18n = new VerticalEccentricI18n();
  public child: string[] = ['foundation', 'load'];
}
