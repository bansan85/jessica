import { I18nForm } from 'toolbox';

export interface VerticalEccentric<T extends number | string> {
  load: T;
  eccentric: T;
}

export class VerticalEccentricI18n implements I18nForm {
  public number: string[] = ['load', 'eccentric'];
}
